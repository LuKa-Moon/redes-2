#ifndef __LB__
#define __LB__

#include "../headers/tabela_servs.h"

typedef enum politica_e {
    RR,     // 0
    LC,     // 1
    PL_INVALIDA
} Politica;

Politica define_politica(const char *tipo);

int seleciona_RR(Serv_Dados x1, Serv_Dados x2);

int seleciona_LC(Serv_Dados x1, Serv_Dados x2);

void incrementa_RR(int tam);

#endif
