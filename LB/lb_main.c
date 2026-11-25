#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // sleep()
#include "../headers/cliente.h"      //Para se comunicar com os servidores
#include "../headers/continua.h"
#include "../headers/defs.h"
#include "../headers/servidor.h"     //Para se comunicar com os clientes
#include "../headers/tabela_servs.h"
#include "../headers/lb.h"

int main(int argc, char *argv[]) {
// PREPARACAO INICIAL---------------------------------------------------------------------------
    // Verifica uso correto
    if(argc != 3) {
        fprintf(stderr, "Uso correto: %s <nome_tabela_servs> tipo\n", argv[0]);
        return ERROR;
    } 
    // Obter dados da entrada
    define_nome_arq_tabela(argv[1]);
    Politica pl = define_politica(argv[2]);

    if(pl == PL_INVALIDA) {
        fprintf(stderr, "Uso correto: %s <nome_tabela_servs> tipo\n", argv[0]);
        return ERROR;
    }

    int servidores = get_quant_servs();
    if(servidores <= 0) {
        fprintf(stderr, "Erro em get_quant_servs\n");
        return ERROR;
    }

    int conexoes = get_total_conexoes();
    if(conexoes < 0) {
        fprintf(stderr, "Erro em get_total_conexoes\n");
        return ERROR;
    }

    Serv_Dados *srvs = get_todos_servs(servidores);
    if(srvs == NULL) {
        fprintf(stderr, "Erro em get_todos_servs\n");
        return ERROR;
    }
/*
    printf("PL = %d\n", pl);
    printf("Quantidade de servidores: %d\n", servidores);
    printf("Total de conexões abertas: %d\n", conexoes);
    imprime_srvs(srvs, servidores);
*/
//-------------------------------------------------------------------------------------------------
//PREPARAÇÂO DO SOCKET PARA ESCUTAR----------------------------------------------------------------
    int lb_server = lb_serv_inicio();  // Cria o socket e escuta na porta LB_PORTA_ESCUTA
    if(lb_server < 0) {
        fprintf(stderr, "Erro em lb_serv_inicio\n");
        return ERROR;
    }

    serv_define_nome("Load Balancer");
//-------------------------------------------------------------------------------------------------
//INICIA O LOOP PRINCIPAL DO LOAD BALANCER---------------------------------------------------------
    // mantem_ate_SIGINT, que permite encerrar graciosamente com ^C
    continua_ini();
    lb_loop_principal(lb_server, srvs, servidores, pl);  // Aceita conexões e trata mensagens

//-------------------------------------------------------------------------------------------------

    // Inicializa variáveis para poder usar
    // mantem_ate_SIGINT, que permite encerrar graciosamente com ^C

    // Definem nomes para impressão no log
    cli_define_nome("Load Balancer");

    // Define o nome do arquivo que será usado pelo tabela servs
    //define_nome_arq_tabela("servs.tab");

// Mantém o LB em execução
while(serv_continua()) {
    sleep(1);
}

 
    free_todos_servs(srvs, servidores);
    if(fecha_socket(lb_server) < 0) {
        fprintf(stderr, "Erro em fecha_socket\n");
        return ERROR;
    }

    return NOERROR;
}