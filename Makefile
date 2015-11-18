include Makefile.in

ifndef OPT
	OPT = 0
endif

TARGS = mapleall mozjs src tests

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

clean: 
	$(MAKE) clean -C src
	$(MAKE) clean -C tests
	$(MAKE) clean -C ../mapleall

.PHONY: $(TARGS)

