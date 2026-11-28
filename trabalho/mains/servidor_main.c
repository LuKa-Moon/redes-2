#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/defs.h"
#include "../headers/continua.h"
#include "../headers/servidor.h"
#include "../headers/tabela_servs.h"

int id;

void serv_inicia_conexao() {}
void serv_finaliza_conexao() {}

int serv_continua() {
    return mantem_ate_SIGINT();
}

int serv_mantem_conexao(char *mensagem) {
    return 0;
}

void serv_trata_mensagem(char *mensagem) {
    if(mensagem == NULL || strlen(mensagem) == 0)
        return;

    incrementa_conexoes(id);
    sleep(rand() % SONO);
    extern char *serv_nome;
    serv_responde(serv_nome);
    decrementa_conexoes(id);
}

int main(int argc, char **argv) {
    // Verifica uso correto
    if(argc != 4) {
        fprintf(stderr, "Uso correto: %s <id_serv> <porta> <tabela_servs>\n", argv[0]);
        return ERROR;
    }
    id = atoi(argv[1]);
    if(!id) {
        fprintf(stderr, "<id_serv> deve ser um inteiro >= 1\n");
        return ERROR;
    }

    char *serv_nome = malloc(sizeof(char) * TAM_MAX);
    sprintf(serv_nome, "Servidor %d", id);
    serv_define_nome(serv_nome);

    define_nome_arq_tabela(argv[3]);

    continua_ini();
    
    status_serv_up(id);

    int retorno = serv_escuta(argv[2]);

    reseta_conexoes(id);
    status_serv_down(id);
    free(serv_nome);
    return retorno;
}