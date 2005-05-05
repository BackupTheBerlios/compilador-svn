#
# Makefile para projetos com o gcc
# 
# 
# Descrição:
# 	Esse `makefile´ gera, no diretório especificado na variável ``BIN´´,
# 	o arquivo EXE do projeto com nome ``projeto´´.
# 	
# 	Os arquivos .c são automaticamente incluídos. Se algum
# 	arquivo com essa extensão não fizer parte do projeto, mova-o para
# 	outro diretório ou renomeie-o para .c.fora, por exemplo.
#
# 	O alvo (`target´) ``clean´´ remove todos os arquivos gerados
# 	por este `makefile´.
#
# Histórico:
# 	21/11/2004	André Pinto		Versão inicial adaptado da versão SDCC
#

################
# Projeto (.exe)
################
projeto=teste

########################
# Diretório dos Binários
########################
BIN=bin
OBJDIR=obj

###################
# Parâmetros do gcc
###################
CFLAGS=-g -Wall
LIBS=

###############################
# Executáveis do GCC
###############################
CC=gcc

#################################
#                               #
# Daqui em diante é intocável!! #
#                               #
#################################

EXE       := $(BIN)/$(projeto)
OBJS      := $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c))

all: $(EXE)

$(EXE): $(OBJS)
	@echo Linkando tudo em $(EXE)
	@$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(EXE)

$(OBJS): $(OBJDIR)/%.o: %.c %.h
	@echo Compilando $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Removendo $(OBJDIR)/*
	@rm -f $(OBJDIR)/*
	@echo Removendo executável $(EXE)
	@rm -f $(EXE)

.PHONY: clean

