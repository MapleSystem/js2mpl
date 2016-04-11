include Makefile.in

TARGS = mapleall mozjs src tests
TARGS = mapleall src tests

all: $(TARGS)
build: mapleall src
test: tests
arm: maplearm src tests
arm2: maplearm2 src tests
clang: mapleallclang src tests

mapleall:
	$(MAKE) -C ../mapleall DEBUG=1

mapleallclang:
	$(MAKE) -C ../mapleall DEBUG=1 USECLANG=1

maplearm:
	$(MAKE) -C ../mapleall ARM=1

maplearm2:
	$(MAKE) -C ../mapleall arm

mozjs:
	$(MAKE) -C ../mozjs

src:
	$(MAKE) -C $@

tests:
	$(MAKE) -C $@ regression

todo:
	$(MAKE) -C tests todo

clean: 
	$(MAKE) clean -C src
	$(MAKE) clean -C tests

cleanall:
	rm -rf build/$(FLAVOR) ../mapleall/build/$(FLAVOR)

cleanallall:
	rm -rf build ../mapleall/build

rebuild: cleanall all

.PHONY: $(TARGS)

