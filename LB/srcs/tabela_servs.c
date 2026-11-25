#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/tabela_servs.h"

#define TAB_NUM_SERVS              48
#define TAB_TOTAL_CONEX            47
#define TAB_ID                      2
#define TAB_CONEX                   8
#define TAB_HOST                   19
#define TAB_PORTA                  37
#define TAB_STATUS                 45
#define TAB_TAM_LINHA              54
#define TAB_INI_DADOS               6

#define ERROR -1
#define NOERROR 0

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

char *NOME_ARQ_TABELA;

/* FUNÇÕES INTERNAS */

int  acha_linha(FILE *tabela, int posicao) {
    return fseek(tabela, TAB_TAM_LINHA * (TAB_INI_DADOS + posicao - 1) + 3, SEEK_SET);
}

char *le_linha(FILE *tabela, int posicao) {
    char *linha = malloc(sizeof(char) * (TAB_TAM_LINHA + 1));
    if(linha == NULL) {
        fprintf(stderr, "Erro ao alocar memória para linha\n");
        return NULL;
    }
    linha[TAB_TAM_LINHA] = '\0';
    
    if(acha_linha(tabela, posicao) != 0) {
        fprintf(stderr, "Erro ao achar linha\n");
        free(linha);
        return NULL;
    }
    
    if(fread(linha, sizeof(char), TAB_TAM_LINHA, tabela) != TAB_TAM_LINHA) {
        fprintf(stderr, "Erro ao ler linha\n");
        free(linha);
        return NULL;
    }
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

void define_nome_arq_tabela(char *nome) {
    NOME_ARQ_TABELA = nome;
    printf("Arquivo de tabela de servidores definido como: %s\n", NOME_ARQ_TABELA);
}

int get_quant_servs() {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r");
    if(tabela == NULL) {
        fprintf(stderr, "Erro ao abrir tabela de servidores: %s\n", NOME_ARQ_TABELA);
        return NOERROR;
    }
    int total = get_quant_servs_interna(tabela);
    if(total <= 0) {
        fprintf(stderr, "Erro: quantidade de servidores inválida\n");
        fclose(tabela);
        return NOERROR;
    }
    fclose(tabela);
    return total;
}

int get_total_conexoes() {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r");
    if(tabela == NULL) {
        fprintf(stderr, "Erro ao abrir tabela de servidores: %s\n", NOME_ARQ_TABELA);
        return NOERROR;
    }
    int total;
    if(fseek(tabela,  2 * TAB_TAM_LINHA + TAB_TOTAL_CONEX, SEEK_SET) != 0) {
        fprintf(stderr, "Erro ao procurar total de conexões\n");
        fclose(tabela);
        return NOERROR;
    }
    if(fscanf(tabela, "%d", &total) != 1) {
        fprintf(stderr, "Erro ao ler total de conexões\n");
        fclose(tabela);
        return NOERROR;
    }
    fclose(tabela);
    return total;
}

Serv_Dados get_dados_serv(int id) {
    FILE *tabela = fopen(NOME_ARQ_TABELA, "r");

    char *linha = le_linha(tabela, id);
    printf("%s", linha);

    Serv_Dados sd = get_dados_serv_interna(linha);

    fclose(tabela);
    return sd;
}

void imprime_srvs(Serv_Dados *srvs, int quantidade) {//--------------------------------------
    for(int i = 0; i < quantidade; i++) {
        printf("ID: %d\n", srvs[i].id);
        printf("Conexões: %d\n", srvs[i].conexoes);
        printf("Host: %s\n", srvs[i].host);
        printf("Porta: %s\n", srvs[i].porta);
        printf("Status: %s\n", srvs[i].status == UP ? "UP" : "DOWN");
        printf("\n");
    }
}

Serv_Dados *get_todos_servs(int quantidade){//-------------------------------------------
    if(quantidade <= 0) {
        fprintf(stderr, "Erro: quantidade inválida\n");
        return NULL;
    }

    FILE *tabela = fopen(NOME_ARQ_TABELA, "r");
    if(tabela == NULL) {
        fprintf(stderr, "Erro ao abrir tabela de servidores: %s\n", NOME_ARQ_TABELA);
        quantidade = 0;
        return NULL;
    }

    Serv_Dados *srvs = malloc(sizeof(Serv_Dados) * quantidade);
    if(srvs == NULL) {
        fprintf(stderr, "Erro ao alocar memória para servidores\n");
        fclose(tabela);
        quantidade = 0;
        return NULL;
    }

    for(int i = 1; i <= quantidade; i++) {
        char *linha = le_linha(tabela, i);
        if(linha == NULL) {
            fprintf(stderr, "Erro ao ler linha %d da tabela\n", i);
            free_todos_servs(srvs, i - 1);
            fclose(tabela);
            quantidade = 0;
            return NULL;
        }
        srvs[i - 1] = get_dados_serv_interna(linha);
        free(linha);
    }

    fclose(tabela);
    return srvs;
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
            desaloca_dado(&melhor);
            melhor = item;
        }
        else
            desaloca_dado(&item);
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

void desaloca_dado(Serv_Dados *sd) {
    free(sd->host);
    free(sd->porta);
}

void free_todos_servs(Serv_Dados *srvs, int quantidade){//--------------------------------------
    for(int i = 0; i < quantidade; i++) {
        desaloca_dado(&srvs[i]);
    }
    free(srvs);
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