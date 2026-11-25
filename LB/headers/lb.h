#ifndef __LB__
#define __LB__

#include "../headers/tabela_servs.h"

typedef enum politica_e {
    RR,     // 0
    LC,     // 1
    PL_INVALIDA
} Politica;

Politica define_politica(const char *tipo);

int seleciona_RR(Serv_Dados *srvs, int servidores);

int seleciona_LC(Serv_Dados *srvs, int servidores);

#endif