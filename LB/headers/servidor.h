#ifndef __SERVIDOR__
#define __SERVIDOR__

#include "../headers/tabela_servs.h"
#include "../headers/lb.h"

// CHAMAR
// Nome para ser usado no log
void serv_define_nome(char *nome);

// Função para fechar o socket
int fecha_socket(int sockfd);

// Chame essa função aqui
// Cria um servidor
// Retorna socket ou NOERROR (0)
int lb_serv_inicio();

// Função para ser usada no laço de escuta
int serv_continua();

void serv_trata_mensagem(char *mensagem);

// Deve retornar 0 para cortar a conexão com o cliente atual
int serv_mantem_conexao(char *mensagem);

// Selecionar um servidor
int serv_seleciona_servidor();

// Função Principal do LB
void lb_loop_principal(int sockfd, Serv_Dados *srvs, int servidores, Politica pl); ;

// Você pode usar essa função para responder no serv_trata_mensagem
// Se resposta == NULL, não faz nada
void serv_responde(int clientfd, char *resposta);

#endif