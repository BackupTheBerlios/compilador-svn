#
# Makefile raiz (roda outros Makefiles)
# 

DIRS=src 

#
# NÃO MEXER A PARTIR DAQUI
#

all: .DEFAULT

.DEFAULT:
	@for d in $(DIRS); do \
	  $(MAKE) -C $$d $(MAKECMDGOALS) ; \
	done


# Compila objetos individualmente

%.o:
	@for d in $(DIRS); do \
	  $(MAKE) -C $$d ../$@ ; \
	done

.PHONY: all clean distclean run makerun %.o
