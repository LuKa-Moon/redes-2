CC = gcc
CFLAGS = -Wall -Wextra -g -Iheaders

TARGET = lb
OBJ_DIR = objs

SRCS = lb_main.c \
       srcs/cliente.c \
       srcs/continua.c \
       srcs/lb.c \
       srcs/servidor.c \
       srcs/tabela_servs.c

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean