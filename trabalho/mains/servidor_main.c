#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/defs.h"
#include "../headers/continua.h"
#include "../headers/servidor.h"

int serv_continua() {
    return mantem_ate_SIGINT();
}

int serv_mantem_conexao(char *mensagem) {
    return 0;
}

void serv_trata_mensagem(char *mensagem) {
    if(mensagem == NULL || strlen(mensagem) == 0)
        return;

    sleep(rand() % SONO);
    extern char *serv_nome;
    serv_responde(serv_nome);
}

int main(int argc, char **argv) {
    // Verifica uso correto
    if(argc != 3) {
        fprintf(stderr, "Uso correto: %s <id_serv> <porta>\n", argv[0]);
        return ERROR;
    }
    int id = atoi(argv[1]);
    if(!id) {
        fprintf(stderr, "<id_serv> deve ser um inteiro >= 1\n");
        return ERROR;
    }

    char *serv_nome = malloc(sizeof(char) * TAM_MAX);
    sprintf(serv_nome, "Servidor %d", id);
    define_nome(serv_nome);

    continua_ini();
    
    int retorno = serv_escuta(argv[2]);
    free(serv_nome);
    return retorno;
}