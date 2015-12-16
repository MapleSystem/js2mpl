include Makefile.in

ifndef OPT
	OPT = 0
endif

TARGS = mapleall mozjs src tests
TARGS = mapleall src tests

all: $(TARGS)
build: mapleall src
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

cleanall:
	rm -rf build ../mapleall/build

rebuild: cleanall all

.PHONY: $(TARGS)

