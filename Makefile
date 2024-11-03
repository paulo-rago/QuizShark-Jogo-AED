# Variáveis
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lraylib

# Nome do executável
TARGET = jogo

# Lista de arquivos fontes
SOURCES = main.c # outras_partes.c
OBJECTS = $(SOURCES:.c=.o)

# Regra padrão
all: $(TARGET)

# Como criar o executável
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Como compilar os arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Limpar os arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)
