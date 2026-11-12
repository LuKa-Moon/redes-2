#include <stdlib.h>
#include <stdio.h>
#include "../headers/cliente.h"      //Para se comunicar com os servidores
#include "../headers/continua.h"
#include "../headers/defs.h"
#include "../headers/servidor.h"     //Para se comunicar com os clientes
#include "../headers/tabela_servs.h"

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
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Uso correto: %s <porta>\n", argv[0]);
        return ERROR;
    }

    // Inicializa variáveis para poder usar
    // mantem_ate_SIGINT, que permite encerrar graciosamente com ^C
    continua_ini();

    // Definem nomes para impressão no log
    cli_define_nome("Load Balancer\n");
    serv_define_nome("Load_Balancer\n");

    return serv_escuta(argv[1]);
}