# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11

# Diretórios
SRCDIR = src
INCDIR = includes
OBJDIR = obj
BINDIR = bin

# Arquivos do projeto
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/game

# Regras principais
all: setup $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linkando o programa..."
	$(CC) -o $@ $^ -lncurses -pthread

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Criação dos diretórios
setup:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Limpeza dos arquivos gerados
clean:
	@echo "Limpando arquivos compilados..."
	rm -rf $(OBJDIR)/*.o $(BINDIR)/game

# Limpeza completa
distclean: clean
	@echo "Removendo diretórios gerados..."
	rm -rf $(OBJDIR) $(BINDIR)

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make clean    - Remove os arquivos objetos e binários"
	@echo "  make distclean - Remove os diretórios gerados"
	@echo "  make help     - Mostra esta ajuda"
