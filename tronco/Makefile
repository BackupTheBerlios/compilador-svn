#
# Makefile raiz (roda outros Makefiles)
# 

DIRS=src 

#
# NÃO MEXER A PARTIR DAQUI
#

all clean distclean: $(DIRS)

all: alvo = all

clean: alvo = clean

distclean: alvo = distclean

$(DIRS):
	$(MAKE) -C $@ $(alvo)


# Compila objetos individualmente

%.o:
	@for d in $(DIRS); do \
	  $(MAKE) -C $$d ../$@ ; \
	done

.PHONY: all clean distclean $(DIRS) %.o
