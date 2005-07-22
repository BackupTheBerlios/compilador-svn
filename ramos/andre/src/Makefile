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
#	As dependências são calculadas automaticamente, através dos
#       includes dos fontes.
#
# 	O alvo (`target´) ``clean´´ remove todos os arquivos gerados
# 	por este `makefile´.
#
# Histórico:
#	22/05/2005	André Pinto		Cria pastas automaticamente, executa
# 	05/05/2005	André Pinto		Dependências
# 	21/11/2004	André Pinto		Versão inicial adaptado da versão SDCC
#

################
# Projeto (.exe)
################
projeto=compilaDOR

############
# Diretórios
############
BINDIR=../bin
OBJDIR=../obj
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
PARAM     := $(MAKECMDGOALS:makerun=)
PARAM     := $(PARAM:run=)
#OBJS2     := $(patsubst %.c,%.o,$(wildcard *.c))


all: $(EXE)

ifneq ($(MAKECMDGOALS),distclean)
include $(DEPS)
endif

# Gera executável

$(EXE): $(OBJS)
	@echo Linkando tudo em $(EXE)
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(EXE)


# Gera objetos

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo Compilando $<
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Previne a criação de objetos na pasta atual

$(OBJS2): %.o: $(OBJDIR)/%.o


# Gera dependências de cada .c

.%.d: %.c
	@echo make: Calculando dependências de $<
	@$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


# Limpa objetos e executável

distclean: clean
	@echo Removendo dependências
	@rm -f $(DEPS)

clean:
	@echo Removendo executável
	@rm -f $(EXE) $(EXE).exe
	@echo Removendo objetos
	@rm -f $(OBJS)

run:
	@echo Executando $(projeto) $(PARAM)
	@cd $(BINDIR) && \
	$(EXE) $(PARAM)

makerun: $(EXE) run

.PHONY: all clean distclean run makerun $(OBJS2)

