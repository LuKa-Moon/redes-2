#ifndef __SERVIDOR__
#define __SERVIDOR__

// Chame essa função aqui
// Cria um servidor que escuta da porta porta
// Retorna ERROR (-1) ou NOERROR (0)
int serv_escuta(char *porta);

// Você pode usar essa função para responder no serv_trata_mensagem
// Se resposta == NULL, não faz nada
void serv_responde(char *resposta);

// CHAMAR
// Nome para ser usado no log
void serv_define_nome(char *nome);

// IMPLEMENTAR
extern void serv_trata_mensagem(char *mensagem);

// IMPLEMENTAR
// Função para ser usada no laço de escuta
extern int serv_continua();

// IMPLEMENTAR
// Deve retornar 0 para cortar a conexão com o cliente atual
extern int serv_mantem_conexao(char *mensagem);

#endif