#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "../headers/lb.h"

int server_RR = 0; // Variável global para armazenar o índice do próximo servidor a ser selecionado
#define MAX 2147483647 // Definindo o valor máximo para um inteiro

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

int seleciona_RR(Serv_Dados *srvs, int servidores){
    while(srvs[server_RR].status == DOWN) {
        server_RR = (server_RR + 1) % servidores;
    }
    int indice = server_RR % servidores; // Seleciona o servidor atual com base no índice
    server_RR = (server_RR + 1) % servidores; 
    return indice;
}

int seleciona_LC(Serv_Dados *srvs, int servidores){
    int min = MAX, indice = -1;
    for(int i = 0; i < servidores; i++) {
        if(srvs[i].status == UP) {
            if(srvs[i].conexoes < min) {
                min = srvs[i].conexoes;
                indice = i;
            }
        }
    }
    return indice;  // retorna o índice do servidor com menos conexões ou retorna -1 se todos os servidores estiverem DOWN
}