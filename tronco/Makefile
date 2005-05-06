#
# Makefile raiz (roda outros Makefiles)
# 

DIRS=src 

all clean distclean:
	@for dir in $(DIRS); do \
   	  $(MAKE) -C $$dir $@; \
	done

.PHONY: all clean $(DIRS)
