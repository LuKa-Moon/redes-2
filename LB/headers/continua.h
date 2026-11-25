#ifndef __CONTINUA__
#define __CONTINUA__

// Chame essa função no começo do código
// Retorna ERROR (-1) ou NOERROR (0)
int continua_ini();

// Retorna 1 até receber um SIGINT (^C)
// Depois, retorna 0
int mantem_ate_SIGINT();

#endif