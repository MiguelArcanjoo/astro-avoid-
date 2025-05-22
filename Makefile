# Nome do executável
EXEC = astro-avoid

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lncurses

# Arquivos fonte
SRCS = src/keyboard.c src/main.c src/screen.c src/timer.c
OBJS = $(SRCS:src/%.c=%.o)

# Regra padrão
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean