#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../headers/defs.h"
#include "../headers/continua.h"
#include "../headers/cliente.h" 

int main(int argc, char **argv) {
    // Verifica uso correto
    if(argc != 2) {
        fprintf(stderr, "Uso correto: %s <id_cli>\n", argv[0]);
        return ERROR;
    }
    int id = atoi(argv[1]);
    if(!id) {
        fprintf(stderr, "<id_cli> deve ser um inteiro >= 1\n");
        return ERROR;
    }

    char *cli_nome = malloc(sizeof(char) * TAM_MAX);
    char *resposta = malloc(sizeof(char) * TAM_MAX);
    sprintf(cli_nome, "Cliente %d", id);
    define_nome(cli_nome);
    
    continua_ini();

    while(mantem_ate_SIGINT()) {
        cli_cria_conexao(LOOP_BACK, LB_PORTA_ESCUTA);

        if(cli_envia_mensagem(cli_nome, resposta) == ERROR)
            printf("[%s] não recebeu resposta\n", cli_nome);
        else
            printf("[%s] Resposta recebida: [%s]\n", cli_nome, resposta);
        sleep(rand() % SONO);

        cli_encerra_conexao();
        printf("\n");
    }
    free(cli_nome);
    free(resposta);
}