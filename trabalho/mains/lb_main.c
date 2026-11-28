#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/cliente.h"      //Para se comunicar com os servidores
#include "../headers/continua.h"
#include "../headers/defs.h"
#include "../headers/servidor.h"     //Para se comunicar com os clientes
#include "../headers/tabela_servs.h"
#include "../headers/lb.h"

int (*COMPARACAO_SERVS)(Serv_Dados x1, Serv_Dados x2);
Politica pl;
int servs_travados;

int serv_continua() {
    return mantem_ate_SIGINT();
}

int serv_mantem_conexao(char *mensagem) {
    return 0;
}

void serv_inicia_conexao() {}

void serv_finaliza_conexao() {
    if(pl == RR) {
        for(int i = 0; i < servs_travados; i++)
            incrementa_RR(get_quant_servs());
    }
}

void serv_trata_mensagem(char *mensagem) {
    if(mensagem == NULL || strlen(mensagem) == 0)
        return;

    /*
     Nessa função:
     * Escolhe-se um servidor com as funções do tabela_servs.h
     * Comunica-se com o servidor com as funções do cliente.h
     * (iniciar e encerrar a conexão)
     * Enviar o retorno para o cliente com a função serv_responde
    */

    // Verificar se tem servidores ativos antes de escolher 
    if(tem_servidores_ativos() == 0){
        printf("Nenhum Servidor esta ativo\n");
        return;
    }

    Serv_Dados sd;
    // sd.id = -1; // Fazer retornar erro no primeiro status_serv_down

    // do{
    //     status_serv_down(sd.id);
    // // Escolhendo o servidor com base em COMPARACAO_SERVS
    // sd = melhor_serv(COMPARACAO_SERVS);
    
    // // o melhor servidor pode estar desativado
    // }while(sd.status == DOWN && mantem_ate_SIGINT());
/*
    // Trata do num em RR
    if(pl == RR){
        incrementa_RR(get_quant_servs());
    }
*/
    char *resposta = malloc(sizeof(char) * TAM_MAX);
    if(resposta == NULL){
        printf("Erro na alocação 'resposta'\n");
    }

    servs_travados = 0;
    do {
        // Escolhendo o servidor com base em COMPARACAO_SERVS
        sd = melhor_serv(COMPARACAO_SERVS);
        printf("servidor escolhido = %d\n", sd.id);

        cli_cria_conexao(sd.host, sd.porta);
        cli_envia_mensagem(mensagem, resposta);
        cli_encerra_conexao();

        // Caso o servidor nao responder
        if(strlen(resposta) == 0) {
            status_serv_down(sd.id);
            if(pl == RR) {
                incrementa_RR(get_quant_servs());
            }
            servs_travados++;
            reseta_conexoes(sd.id);
        }
            
    } while(strlen(resposta) == 0 && mantem_ate_SIGINT() && tem_servidores_ativos());
    
    // Verificar se tem servidores ativos antes de responder 
    if(tem_servidores_ativos() == 0){
        printf("Nenhum Servidor esta ativo\n");
        return;
    }

    // achou o servidor
    serv_responde(resposta);   

    free(resposta);
}

int main(int argc, char *argv[]) {
    // Verifica uso correto
    if(argc != 4) {
        fprintf(stderr, "Uso correto: %s <nome_tabela_servs> <tipo_politica> <porta>\n", argv[0]);
        return ERROR;
    } 

    // Inicializa variáveis para poder usar
    // mantem_ate_SIGINT, que permite encerrar graciosamente com ^C
    continua_ini();

    // Definem nomes para impressão no log
    cli_define_nome("Load Balancer");
    serv_define_nome("Load Balancer");

    // Define o nome do arquivo que será usado pelo tabela servs
    define_nome_arq_tabela(argv[1]);

    // Escolher funcao de comparacao de servs
    pl = define_politica(argv[2]);
    if(pl == PL_INVALIDA) {
        fprintf(stderr, "Uso correto: %s <nome_tabela_servs> tipo <porta>\n", argv[0]);
        return ERROR;
    }

    if(pl == RR){
        COMPARACAO_SERVS = seleciona_RR;
    }
    else if(pl == LC){
        COMPARACAO_SERVS = seleciona_LC;
    }
    return serv_escuta(argv[3]);
}
