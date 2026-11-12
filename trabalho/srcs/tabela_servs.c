#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/tabela_servs.h"

#define NOME_ARQ_TABELA "servs.tab"

#define TAB_NUM_SERVS              48
#define TAB_TOTAL_CONEX            47
#define TAB_ID                      2
#define TAB_CONEX                   8
#define TAB_HOST                   19
#define TAB_PORTA                  37
#define TAB_STATUS                 45
#define TAB_TAM_LINHA              54
#define TAB_INI_DADOS               6

/*
-----------------------------------------------------
| Número de servidores:                         000 |
| Total de conexões:                           0000 |
|---------------------------------------------------|
| ID  | Conexões | Host            | Porta | Status |
|---------------------------------------------------|
| 000 | 0000     | 111.111.111.111 | 00000 | DOWN   |
-----------------------------------------------------
*/

/* FUNÇÕES INTERNAS */

void acha_linha(FILE *tabela, int id) {
    fseek(tabela, TAB_TAM_LINHA * (TAB_INI_DADOS + id - 1) + 3, SEEK_SET);
}

char *le_linha(FILE *tabela, int id) {
    char *linha = malloc(sizeof(char) * (TAB_TAM_LINHA + 1));
    linha[TAB_TAM_LINHA] = '\0';

    acha_linha(tabela, id);
    fread(linha, sizeof(char), TAB_TAM_LINHA, tabela);
    return linha;
}

int pega_num(char *linha, int ini) {
    linha += ini;
    int num;
    char *fim = strchr(linha, ' ');
    fim[0] = '\0';
    num = atoi(linha);
    fim[0] = ' ';
    return num;
}

char *pega_string(char *linha, int ini) {
    linha += ini;
    char *fim = strchr(linha, ' ');
    fim[0] = '\0';
    char *result = malloc(TAB_TAM_LINHA * sizeof(char));
    strcpy(result, linha);
    fim[0] = ' ';
    return result;
}

Status pega_status(char *linha) {
    if(strstr(linha, "UP"))
        return UP;
    return DOWN;
}

int get_quant_servs_interna(FILE *tabela) {
    int total;
    fseek(tabela, TAB_TAM_LINHA + TAB_NUM_SERVS, SEEK_SET);
    fscanf(tabela, "%d", &total);
    return total;
}

Serv_Dados get_dados_serv_interna(char *linha) {
    Serv_Dados sd;
    sd.id = pega_num(linha, TAB_ID);
    sd.conexoes = pega_num(linha, TAB_CONEX);
    sd.host = pega_string(linha, TAB_HOST);
    sd.porta = pega_string(linha, TAB_PORTA);
    sd.status = pega_status(linha);
    return sd;
}

/* FUNÇÕES EXTERNAS */

int get_quant_servs() {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    int total = get_quant_servs_interna(tabela);
    fclose(tabela);
    return total;
}

int get_total_conexoes() {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    int total;
    fseek(tabela,  2 * TAB_TAM_LINHA + TAB_TOTAL_CONEX, SEEK_SET);
    fscanf(tabela, "%d", &total);
    fclose(tabela);
    return total;
}

Serv_Dados get_dados_serv(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");

    char *linha = le_linha(tabela, id);
    printf("%s", linha);

    Serv_Dados sd = get_dados_serv_interna(linha);

    fclose(tabela);
    return sd;
}

Serv_Dados melhor_serv(int (*comp)(Serv_Dados x1, Serv_Dados x2)) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    char *linha = le_linha(tabela, 1);
    Serv_Dados melhor = get_dados_serv_interna(linha);

    int n = get_quant_servs_interna(tabela);

    for(int i = 2; i <= n; i++) {
        acha_linha(tabela, i);
        fread(linha, sizeof(char), TAB_TAM_LINHA, tabela);
        Serv_Dados item = get_dados_serv_interna(linha);
        if(comp(melhor, item) > 0) {
            desaloca_dado(melhor);
            melhor = item;
        }
        else
            desaloca_dado(item);
    }

    free(linha);
    fclose(tabela);
    return melhor;
}

void flip_status_serv(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    char *linha = le_linha(tabela, id);
    Status status = pega_status(linha);

    acha_linha(tabela, id);
    fseek(tabela, TAB_STATUS, SEEK_CUR);
    
    if(status == UP)
        fprintf(tabela, "DOWN");
    else
        fprintf(tabela, "UP  ");
    fclose(tabela);
}

void incrementa_conexoes(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    char *linha = le_linha(tabela, id);
    int conex = pega_num(linha, TAB_CONEX);

    acha_linha(tabela, id);
    fseek(tabela, TAB_CONEX, SEEK_CUR);
    conex++;
    fprintf(tabela, "%04d", conex);
    fclose(tabela);
}

void decrementa_conexoes(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    char *linha = le_linha(tabela, id);
    int conex = pega_num(linha, TAB_CONEX);

    acha_linha(tabela, id);
    fseek(tabela, TAB_CONEX, SEEK_CUR);
    conex--;
    fprintf(tabela, "%04d", conex);
    fclose(tabela);
}

void reseta_conexoes(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r+");
    acha_linha(tabela, id);
    fseek(tabela, TAB_CONEX, SEEK_CUR);
    fprintf(tabela, "0000");
    fclose(tabela);
}

void desaloca_dado(Serv_Dados sd) {
    free(sd.host);
    free(sd.porta);
}

#define LINHA_HORIZONTAL   "-----------------------------------------------------\n"
#define CAMPO_NUM_SERVS    "| Número de servidores:                         "
#define LINHA_QUANT_CONECT "| Total de conexões:                           0000 |\n"
#define CABECALHO          "| ID  | Conexões | Host            | Porta | Status |\n"
void gera_tabela_servs(int n) {
/*
-----------------------------------------------------
| Número de servidores:                         000 |
| Total de conexões:                           0000 |
|---------------------------------------------------|
| ID  | Conexões | Host            | Porta | Status |
|---------------------------------------------------|
| 000 | 0000     | 111.111.111.111 | 00000 | DOWN   |
-----------------------------------------------------
*/
    printf(LINHA_HORIZONTAL);
    printf(CAMPO_NUM_SERVS);
    printf("%3d |\n", n);
    printf(LINHA_QUANT_CONECT);
    printf(LINHA_HORIZONTAL);
    printf(CABECALHO);
    printf(LINHA_HORIZONTAL);

    for(int i = 1; i < n; i++) {
        printf("| %3d | 0000     | 127.0.1.1       | %5d | UP     |\n",
                   i,                             i + 8500);
    }
    
    printf(LINHA_HORIZONTAL);
}