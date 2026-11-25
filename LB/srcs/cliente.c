#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // close, read, write
#include <sys/types.h>   // Tipos de dados (socklen_t)
#include <sys/socket.h>  // Funções e estruturas de socket
#include <netinet/in.h>  // Estruturas de endereço IP (sockaddr_in)
#include <netdb.h>       // getaddrinfo, addrinfo
#include "../headers/defs.h"
#include "../headers/cliente.h"

char *cli_nome;

void cli_define_nome(char *nome) {
    cli_nome = nome;
}

int cli_cria_conexao(char *host, char *porta) {
    struct addrinfo hints,  // Define o tipo de endereço do servidor (TCP + IPv4)
                    *res;
    
    // Inicialização da estrutura hints
    memset(&hints, 0, sizeof(hints));

    // Configura os parâmetros do endereço do servidor: TCP + IPv4
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    // Obtém estrutura de endereço do servidor
    if (getaddrinfo(host, porta, &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo\n");
        return ERROR;
    }

    // Cria socket TCP
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        fprintf(stderr, "socket\n");
        freeaddrinfo(res);
        return ERROR;
    }

    // Conecta ao servidor
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        //fprintf(stderr, "connect\n");
        close(sockfd);
        freeaddrinfo(res);
        return ERROR;
    }

    // Libera memória da estrutura res
    freeaddrinfo(res);

    printf("[%s] Cliente conectado\n", cli_nome);
    return sockfd;
}

void cli_encerra_conexao(int sockfd) {
    // Fecha socket
    printf("[%s] Cliente encerrado\n", cli_nome);
    close(sockfd);
}

int cli_envia_mensagem(int sockfd, char *envio, char *resposta) {
    char *buffer = malloc(sizeof(char) * TAM_MAX);
    int byte_recebido;

    // Lê mensagem do usuário
    printf("[%s] Enviou uma mensagem: [%s]\n", cli_nome, envio);
    memcpy(buffer, envio, strlen(envio));

    // Envia mensagem para o servidor
    printf("servfd = %d\n", sockfd);
    if(send(sockfd, buffer, strlen(buffer), 0) == ERROR)
        return ERROR;

    // Recebe resposta do servidor
    byte_recebido = recv(sockfd, buffer, TAM_MAX, 0);
    if(byte_recebido < 0) {
        // FAZER TRATAMENTO DE ERRO AQUI
        fprintf(stderr, "trata_conexao recv\n");
        return ERROR;
    }
    else if(byte_recebido == 0) {
        printf("    [%s] Conexão encerrada pelo cliente\n", cli_nome);
        return ERROR;
    }

    memcpy(resposta, buffer, TAM_MAX);
    free(buffer);
    return NOERROR;
}
