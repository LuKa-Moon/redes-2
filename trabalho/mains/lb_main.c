#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/cliente.h"      //Para se comunicar com os servidores
#include "../headers/continua.h"
#include "../headers/defs.h"
#include "../headers/servidor.h"     //Para se comunicar com os clientes
#include "../headers/tabela_servs.h"

int (*COMPARACAO_SERVS)(Serv_Dados x1, Serv_Dados x2) = NULL;

int serv_continua() {
    return mantem_ate_SIGINT();
}

int serv_mantem_conexao(char *mensagem) {
    return 0;
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

    // Escolhendo o servidor com base em COMPARACAO_SERVS
    Serv_Dados sd = melhor_serv(COMPARACAO_SERVS);
    /*
    // o melhor servidor pode estar desativado
    enquanto(sd.status == DOWN && tem_servidores_ativos() && mantem_ate_SIGINT())
        status_serv_down(sd.id);
        desaloca o dado e tenta denovo

    // achou o servidor
    cli_cria_conexao()
    cli_envia_mensagem(mensagem, resposta) <- alocar resposta
    cli_encerra_conexao()
    serv_responde(resposta)
    */   
}

int main(int argc, char *argv[]) {
    // Verifica uso correto
    if(argc != 2) {
        fprintf(stderr, "Uso correto: %s <nome_tabela_servs>\n", argv[0]);
        return ERROR;
    } 

    // Inicializa variáveis para poder usar
    // mantem_ate_SIGINT, que permite encerrar graciosamente com ^C
    continua_ini();

    // Definem nomes para impressão no log
    cli_define_nome("Load Balancer");
    serv_define_nome("Load Balancer");

    // Define o nome do arquivo que será usado pelo tabela servs
    define_nome_arq_tabela("servs.tab");

    // Escolher funcao de comparacao de servs
    // Sugiro passar por argv
    // COMPARACAO_SERVS = <funcao escolhida>

    return serv_escuta(LB_PORTA_ESCUTA);
}