#ifndef __CLIENTE__
#define __CLIENTE__

// Chamar antes de enviar qualquer mensagem
// Cria conexão com um servidor em host, porta
// Retorna ERROR (-1) ou NOERROR (0)
int cli_cria_conexao(char *host, char *porta);

// Chamar após enviar tidas as mensagens
// Retorna ERROR (-1) ou NOERROR (0)
void cli_encerra_conexao();

// Retorna ERROR (-1) se não enviou ou houve resposta (resposta == NULL) 
// ou NOERROR (0) caso contrário
int cli_envia_mensagem(char *envio, char *resposta);

// CHAMAR
// Nome para ser usado no log
void cli_define_nome(char *nome);

#endif 