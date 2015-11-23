include Makefile.in

ifndef OPT
	OPT = 0
endif

TARGS = mapleall mozjs src tests
TARGS = mapleall src tests

all: $(TARGS)
build: src
test: tests

mapleall:
	$(MAKE) -C ../mapleall

mozjs:
	$(MAKE) -C ../mozjs

src:
	$(MAKE) -C $@

tests:
	$(MAKE) -C $@ regression OPT=$(OPT)

todo:
	$(MAKE) -C tests todo

clean: 
	$(MAKE) clean -C src
	$(MAKE) clean -C tests

cleanall: clean
	$(MAKE) clean -C ../mapleall

rebuild: cleanall all

.PHONY: $(TARGS)

