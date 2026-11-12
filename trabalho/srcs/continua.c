#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "../headers/defs.h"
#include "../headers/continua.h"

volatile sig_atomic_t continua;

void verifica_se_para(int sinal) {
    if(sinal == SIGINT) 
        continua = 0;
}

int continua_ini() {
    struct sigaction sa;

    // Configura a estrutura sigaction
    sa.sa_handler = verifica_se_para;  // Define a função que tratará o sinal
    sigemptyset(&sa.sa_mask);          
    sa.sa_flags = 0;                   

    // Associa o sinal SIGINT (Ctrl+C) à nossa estrutura de tratamento
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "Erro ao configurar o sigaction\n");
        return ERROR;
    }
    continua = 1;
    return NOERROR;
}

int mantem_ate_SIGINT() {
    return continua;
}