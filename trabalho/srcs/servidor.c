#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // close, read, write
#include <sys/types.h>   // Tipos de dados (socklen_t)
#include <sys/socket.h>  // Funções e estruturas de socket
#include <netinet/in.h>  // Estruturas de endereço IP (sockaddr_in)
#include <netdb.h>       // getaddrinfo, addrinfo
#include "../headers/defs.h"
#include "../headers/servidor.h"

int connfd;             // Descritor do socket de conexão
char *serv_nome;

void serv_define_nome(char *nome) {
    serv_nome = nome;
}

void trata_conexao() {
    char *buffer = malloc(sizeof(char) * TAM_MAX);
    do {
        // Recebe dados do cliente
        memset(buffer, 0, TAM_MAX);
        recv(connfd, buffer, TAM_MAX, 0);
        
        printf("    [%s] Mensagem recebida: [%s]\n", serv_nome, buffer);
        serv_trata_mensagem(buffer);
    } while(serv_continua() && serv_mantem_conexao(buffer));

    printf("    [%s] Conexão finalizada\n", serv_nome);
    free(buffer);
    close(connfd);
}

int serv_escuta(char *porta) {
    int sockfd;                                 // Descritor do socket de escuta
    struct addrinfo hints,                      // Define o tipo de endereço do servidor (TCP + IPv4)
                    *res;                  
    struct sockaddr_storage cliente_addr;       // Endereço genérico do cliente
    socklen_t addr_len = sizeof(cliente_addr);  // Tamanho da estrutura de endereço

    // Inicialização da estrutura hints
    memset(&hints, 0, sizeof(hints));

    // Configura os parâmetros do endereço do servidor: TCP + IPv4
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Escuta em todas as interfaces

    // Obtém estrutura de endereço do servidor
    if (getaddrinfo(NULL, porta, &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo\n");
        return ERROR;
    }

    // Cria socket TCP
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        fprintf(stderr, "socket\n");
        freeaddrinfo(res);
        return ERROR;
    }

    // Bind do socket ao endereço local retornado
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        fprintf(stderr, "bind\n");
        close(sockfd);
        freeaddrinfo(res);
        return ERROR;
    }

    // Libera memória da estrutura res
    freeaddrinfo(res);

    // Passa o socket para estado de escuta
    if (listen(sockfd, TAM_FILA) < 0) {
        fprintf(stderr, "listen\n");
        close(sockfd);
        return ERROR;
    }

    // Loop principal, aceita conexões iterativamente
    while (serv_continua()) {
        printf("[%s] Aguardando conexões...\n", serv_nome);

        // Aceita conexão de um cliente
        connfd = accept(sockfd, (struct sockaddr *)&cliente_addr, &addr_len);
        if (connfd < 0) {
            //fprintf(stderr, "accept\n");
            continue;
        }

        printf("[%s] Cliente conectado.\n", serv_nome);

        int garfo = fork();
        if(garfo < 0)
            return ERROR;
        if(garfo == 0) {
            trata_conexao();
            exit(0);
        }  
        // Pai fecha conexão com o cliente
        close(connfd); 
    }

    // Fecha socket de escuta
    close(sockfd);

    printf("[%s] Servidor encerrado\n", serv_nome);
    return NOERROR;
}

void serv_responde(char *resposta) {
    if(resposta == NULL)
        return;
    // Envia a mensagem de resposta

    char *buffer = malloc(sizeof(char) * TAM_MAX);
    memcpy(buffer, resposta, TAM_MAX);
    send(connfd, buffer, TAM_MAX, 0);
    
    printf("    [%s] Resposta enviada: [%s]\n", serv_nome, resposta);
    free(buffer);
}