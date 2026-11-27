#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "../headers/lb.h"

int server_RR = 0; // Variável global para armazenar o índice do próximo servidor a ser selecionado

Politica define_politica(const char *tipo){
    if(strcmp(tipo, "RR") == 0){
        printf("Politica definida como ROUND_ROBIN\n");
        return RR;
    } else if(strcmp(tipo, "LC") == 0){
        printf("Politica definida como LEAST_CONNECTIONS\n");
        return LC;
    } else {
        fprintf(stderr, "Politica desconhecida: %s\n", tipo);
        return PL_INVALIDA; // Valor padrão
    }
}

int seleciona_RR(Serv_Dados x1, Serv_Dados x2){
    if(x1.id == server_RR){
        return -1;
    }
    return 1;
}

int seleciona_LC(Serv_Dados x1, Serv_Dados x2){
    if(x1.conexoes >= x2.conexoes){
        return 1;
    }
    return -1;
}

void incrementa_RR(int tam){
    server_RR = (server_RR + 1)% tam;
}