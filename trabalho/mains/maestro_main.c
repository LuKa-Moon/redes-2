#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "../headers/continua.h"
#include "../headers/tabela_servs.h"

#define NOME_EXEC_SERV "./servidor"
#define NOME_EXEC_CLI  "./cliente"
#define NOME_EXEC_LB  "./lb"

int num_servs = 1;
int num_clis  = 1;
int round_robin = 0;
int proc_atual = 0;
char *nome_tab;
int *procs_criados;

void registra_opts(int argc, char **argv) {
    int opt = getopt(argc, argv, "s:c:rt:");
    while(opt > 0) {
        switch (opt) {
        case 's':
            num_servs = atoi(optarg);
            break;
        case 'c':
            num_clis = atoi(optarg);
            break;
        case 'r':
            round_robin = 1;
            break;
        case 't':
            strcpy(nome_tab, optarg);
            break;
        default:
            break;
        }
        opt = getopt(argc, argv, "s:c:rt:");
    }
    if(strlen(nome_tab) == 0) {
        printf("necessário passar nome para tabela com -t\n");
        exit(1);
    }
}

void cria_processo(char *comando, char **co_args) {
    procs_criados[proc_atual] = fork();
    if(procs_criados[proc_atual] == 0) {
        execv(comando, co_args);
        exit(0);
    }
    proc_atual++;
}

int main(int argc, char **argv) {
    continua_ini();
    nome_tab = malloc(32);
    nome_tab[0] = 0;

    registra_opts(argc, argv);
    define_nome_arq_tabela(nome_tab);

    srand(time(NULL));
    int lb_porta = (rand() % 100) * 10 + 8000;

    gera_tabela_servs(num_servs, lb_porta);

    char **args = malloc(5 * sizeof(char*));
    args[0] = NOME_EXEC_SERV;
    args[1] = malloc(50);
    args[2] = malloc(50);
    args[3] = malloc(50);
    args[4] = NULL;

    procs_criados = malloc(sizeof(int) * (num_servs + num_clis + 2));

    strcpy(args[3], nome_tab);
    for(int i = 1; i <= num_servs; i++) {
        Serv_Dados sd = get_dados_serv(i);
        sprintf(args[1], "%d", i);
        strcpy(args[2], sd.porta);
        cria_processo(NOME_EXEC_SERV, args);
        desaloca_dado(sd);
    }

    sleep(2);

    args[0] = NOME_EXEC_LB;
    strcpy(args[1], nome_tab);
    if(round_robin)
        strcpy(args[2], "RR");
    else
        strcpy(args[2], "LC");
    sprintf(args[3], "%d", lb_porta);
    cria_processo(NOME_EXEC_LB, args);

    free(args[3]);
    args[3] = NULL;

    args[0] = NOME_EXEC_CLI;
    sprintf(args[2], "%d", lb_porta);
    sleep(2);
    for(int i = 1; i <= num_clis; i++) {
        sprintf(args[1], "%d", i);
        cria_processo(NOME_EXEC_CLI, args);
    }

    while(mantem_ate_SIGINT()) {
        int d;
        scanf("%d", &d);
        printf("O scanf funciona %d\n", d);
    }

    for(int i = 0; i < proc_atual; i++) {
        if(procs_criados[i] > 0) {
            kill(procs_criados[i], SIGINT);
        }
    }

    sleep(1);
    for(int i = 1; i <= num_servs; i++)
        reseta_conexoes(i);
    
    free(args[2]);
    free(args[1]);
    free(args);
    return 0;
}