#ifndef __TABELA_SERVS__

typedef enum status_e {UP, DOWN} Status;

typedef struct dado_tabela_servidores {
    int id;                 // um inteiro entre 1 e n
    int conexoes;           // quantidade de conexões abertas no servidor
    char *host, *porta;     // ip e porta do servidor
    Status status;          // UP (tudo certo) ou DOWN (falhou)

} Serv_Dados;

// Informa a quantidade de servidores 
int get_quant_servs();

// Informa o total de conexões abertas
int get_total_conexoes();

// Lê os dados de um servidor
Serv_Dados get_dados_serv(int id);

// Essa função varre a tabela e acha o "melhor" servidor
// Para achar o melhor usa a função comp
// Comp deve funcionar da seguinte forma:
//      retorno < 0 -> escolha x1;
//      retorno > 0 -> escolha x2;
//      retorno = 0 -> tanto faz.
Serv_Dados melhor_serv(int (*comp)(Serv_Dados x1, Serv_Dados x2));

// Flipa o status do servidor
void flip_status_serv(int id);

// Faz conexoes++ no servidor
void incrementa_conexoes(int id);

// Faz conexoes-- no servidor
void decrementa_conexoes(int id);

// Faz conexoes == 0 no servidor
void reseta_conexoes(int id);

// Gera uma tabela de servidores de tamanho n no stdout
void gera_tabela_servs(int n);

// Desaloca as string de host e porta
void desaloca_dado(Serv_Dados);

#define __TABELA_SERVS__
#endif