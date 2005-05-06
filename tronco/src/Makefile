#
# Makefile para projetos com o gcc
# 
# 
# Descrição:
# 	Esse `makefile´ gera, no diretório especificado na variável ``BINDIR´´,
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
# 	05/05/2005	André Pinto		Dependências
# 	21/11/2004	André Pinto		Versão inicial adaptado da versão SDCC
#

################
# Projeto (.exe)
################
projeto=compilador

############
# Diretórios
############
BINDIR=../bin
OBJDIR=../objlin
SRCDIR=.

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

EXE       := $(BINDIR)/$(projeto)
FONTES    := $(wildcard $(SRCDIR)/*.c)
OBJS      := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(FONTES))
DEPS      := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/.%.d,$(FONTES))
OBJS2     := $(patsubst %.c,%.o,$(FONTES))
#OBJS2     := $(patsubst %.c,%.o,$(wildcard *.c))


all: $(EXE)

include $(DEPS)

# Gera executável

$(EXE): $(OBJS)
	@echo Linkando tudo em $(EXE)
	@$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(EXE)


# Gera objetos

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo Compilando $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Previne a criação de objetos na pasta atual

$(OBJS2): %.o: $(OBJDIR)/%.o


# Gera dependências de cada .c

.%.d: %.c
	@echo Calculando dependências de $<
	@$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


# Limpa objetos e executável

distclean: clean
	@echo Removendo dependências
	@rm -f $(DEPS)

clean:
	@echo Removendo executável
	@rm -f $(EXE)
	@echo Removendo objetos
	@rm -f $(OBJS)

.PHONY: all clean $(OBJS2)

