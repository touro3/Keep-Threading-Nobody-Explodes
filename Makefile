# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Target binary
TARGET = $(BINDIR)/game

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Rules
all: setup $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linkando o programa..."
	$(CC) $(CFLAGS) -o $@ $^ -lncurses

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

setup:
	@mkdir -p $(BINDIR)

run: $(TARGET)
	@echo "Iniciando o jogo..."
	./$(TARGET)

clean:
	@echo "Limpando arquivos compilados..."
	rm -rf $(OBJDIR)/*.o $(TARGET)

distclean: clean
	@echo "Removendo diretórios..."
	rm -rf $(OBJDIR) $(BINDIR)
