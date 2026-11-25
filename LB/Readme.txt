Load Balancer (LB)

[x] Fazer leitura da tabela de servidor recebido na entrada
[x] Criação do Socket
[x] Criação de ambiente para receber msg do cliente
[ ] Criação de ambiente para receber msg de varios clientes
[ ] Criação de ambiente para conectar com servidores
[ ] Implementar RR
[ ] Implementar LC


x 1.Receber conexões: terminar o tratamento de accept().
 2.Selecionar servidor: implementar a escolha RR e LC.
3.Conectar ao servidor: utilizar connect().
4.Encaminhar dados: cliente → LB → servidor.
5.Encaminhar respostas: servidor → LB → cliente.
6.Concorrência: permitir múltiplas conexões simultâneas.
7.Gerenciamento: atualizar contadores e tratar encerramentos.