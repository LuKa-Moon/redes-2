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
#include "../headers/continua.h"

char *serv_nome;

void serv_define_nome(char *nome) {
    serv_nome = nome;
}

int fecha_socket(int sockfd) {
    if(close(sockfd) < 0) {
        fprintf(stderr, "close\n");
        return ERROR;
    }
    return NOERROR;
}

int lb_serv_inicio() {
    int sockfd;                                 // Descritor do socket de escuta
    struct addrinfo hints,                      // Define o tipo de endereço do servidor (TCP + IPv4)
                    *res;                  

    // Inicialização da estrutura hints
    memset(&hints, 0, sizeof(hints));

    // Configura os parâmetros do endereço do servidor: TCP + IPv4
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // Escuta em todas as interfaces

    // Obtém estrutura de endereço do servidor
    if (getaddrinfo(NULL, LB_PORTA_ESCUTA, &hints, &res) != 0) {
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

    return sockfd;
}

int serv_continua() {
    return mantem_ate_SIGINT();
}

void serv_trata_mensagem(char *mensagem) {
    if(mensagem == NULL || strlen(mensagem) == 0)
        return;

    /*
     Nessa função:
     * Escolhe-se um servidor com as funções do tabela_servs.h
     * Comunica-se com o servidor com as funções do cliente.h
     * (iniciar e encerrar a conexão)
     * Enviar o retorno para o cliente com a função serv_responde
    */
}

int serv_mantem_conexao(char *mensagem) {
    if(mensagem == NULL || strlen(mensagem) == 0 || strcmp(mensagem, "exit") == 0) {
        return 0; // Retorna 0 para cortar a conexão
    }
    return 1; // Retorna 1 para manter a conexão
}

void trata_conexao(int clientfd) {
    char *buffer = malloc(sizeof(char) * TAM_MAX);
    if(buffer == NULL) {
        fprintf(stderr, "trata_conexao malloc\n");
        close(clientfd);
        return;
    }

    ssize_t bytes_recebidos;
    do {
        // Recebe dados do cliente
        memset(buffer, 0, TAM_MAX);
        bytes_recebidos = recv(clientfd, buffer, TAM_MAX - 1, 0);
        if(bytes_recebidos < 0) {
            // FAZER TRATAMENTO DE ERRO AQUI
            fprintf(stderr, "trata_conexao recv\n");
            break;
        }
        else if(bytes_recebidos == 0) {
            printf("    [%s] Conexão encerrada pelo cliente\n", serv_nome);
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
        printf("    [%s] Mensagem recebida: [%s]\n", serv_nome, buffer);
        serv_trata_mensagem(buffer);
    } while(serv_continua() && serv_mantem_conexao(buffer));

    printf("    [%s] Conexão finalizada\n", serv_nome);
    free(buffer);
    close(clientfd);
}

void lb_loop_principal(int sockfd, Serv_Dados *srvs, int servidores, Politica pl) {
    struct sockaddr_in cli_addr;  // Estrutura para armazenar o endereço do cliente
    socklen_t cli_len;  // Tamanho da estrutura do endereço do cliente
    int clientfd,  // Descritor do socket de conexão com cliente
        sevidor_selecionado;  // ID do servidor selecionado
    

    while (serv_continua()) {
        printf("[%s] Aguardando conexões...\n", serv_nome);

        cli_len = sizeof(cli_addr);
        clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (clientfd < 0) {
            if (!serv_continua())
                break;

            fprintf(stderr, "accept\n");
            continue; 
        }

        printf("    [%s] Conexão aceita\n", serv_nome);
        // escolhe um servidor e trata a conexão com ele
        if(pl == RR){
             sevidor_selecionado = seleciona_RR(srvs, servidores);
        }
        else if(pl == LC){
            sevidor_selecionado = seleciona_LC(srvs, servidores);
        }

        printf("    [%s] Servidor selecionado: %d\n", serv_nome, sevidor_selecionado);

        trata_conexao(clientfd);
    }
}

void serv_responde(int clientfd, char *resposta) {
    if(resposta == NULL)
        return;
    // Envia a mensagem de resposta

    char *buffer = malloc(sizeof(char) * TAM_MAX);
    memcpy(buffer, resposta, TAM_MAX);
    send(clientfd, buffer, strlen(buffer), 0);
    
    printf("    [%s] Resposta enviada: [%s]\n", serv_nome, resposta);
    free(buffer);
}