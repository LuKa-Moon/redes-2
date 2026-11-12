CFLAGS = -g -Wall
CC = gcc 

all: cliente servidor

cliente: objs/cliente.o objs/continua.o  objs/cliente_main.o
	$(CC) -o cliente objs/cliente.o objs/continua.o  objs/cliente_main.o

servidor: objs/servidor.o objs/continua.o objs/servidor_main.o
	$(CC) -o servidor objs/servidor.o objs/continua.o objs/servidor_main.o

lb: objs/cliente.o objs/servidor.o objs/continua.o objs/servidor_main.o
	$(CC) -o lb objs/cliente.o objs/servidor.o objs/continua.o objs/servidor_main.o


# Compilando mains
objs/cliente_main.o: mains/cliente_main.c
	$(CC) -c $(CFLAGS) -o objs/cliente_main.o mains/cliente_main.c

objs/servidor_main.o: mains/servidor_main.c
	$(CC) -c $(CFLAGS) -o objs/servidor_main.o mains/servidor_main.c

objs/lb_main.o: mains/lb_main.c
	$(CC) -c $(CFLAGS) -o objs/lb_main.o mains/lb_main.c



# Compilando srcs
objs/cliente.o: headers/cliente.h srcs/cliente.c
	$(CC) -c $(CFLAGS) -o objs/cliente.o srcs/cliente.c

objs/continua.o: headers/continua.h srcs/continua.c
	$(CC) -c $(CFLAGS) -o objs/continua.o srcs/continua.c

objs/servidor.o: headers/servidor.h srcs/servidor.c
	$(CC) -c $(CFLAGS) -o objs/servidor.o srcs/servidor.c

clean:
	-rm -f objs/*.o

purge: clean
	-rm -f trab