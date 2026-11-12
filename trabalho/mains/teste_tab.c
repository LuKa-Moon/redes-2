#include <stdlib.h>
#include <stdio.h>
#include "../headers/tabela_servs.h"

int maior_serv(Serv_Dados x1, Serv_Dados x2) {
    if(x2.id > x1.id) return 1;
    return -1;
}

int main() {
    for(int i = 1; i < 5; i++) {
        Serv_Dados sd = get_dados_serv(i);
        printf("serv id = %d\n", sd.id);
        printf("serv conex = %d\n", sd.conexoes);
        printf("serv host = %s\n", sd.host);
        printf("serv porta = %s\n", sd.porta);
        if(sd.status == UP)
            printf("serv up\n");
        else
            printf("serv down\n");
        printf("\n");
    
    }

    printf("Num servs = %d\n", get_quant_servs());
    printf("Quant conex = %d\n", get_total_conexoes());

    Serv_Dados melhor = melhor_serv(maior_serv);
    printf("Melhor servidor encontrado foi o %d\n", melhor.id);

    desaloca_dado(melhor);

    int n;
    scanf("%d", &n);
    gera_tabela_servs(n);
    return 0;
}