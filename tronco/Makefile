#
# Makefile raiz (roda outros Makefiles)
# 

DIRS=src 

#
# NÃO MEXER A PARTIR DAQUI
#

#all clean distclean:
#	@for d in $(DIRS); do \
#   	  $(MAKE) -C $$d $@; \
#	done

all clean distclean: $(DIRS)

all: alvo = all

clean: alvo = clean

distclean: alvo = distclean

$(DIRS):
	$(MAKE) -C $@ $(alvo)



.PHONY: all clean distclean $(DIRS)
