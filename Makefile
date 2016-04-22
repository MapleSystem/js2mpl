include Makefile.in

TARGS = mapleall mozjs src tests
TARGS = mapleall src

all: $(TARGS) regression
build: mapleall src
arm: maplearm src
arm2: maplearm2 src
clang: mapleallclang src

mapleall:
	$(MAKE) -C ../mapleall

mapleallclang:
	$(MAKE) -C ../mapleall USECLANG=1

maplearm:
	$(MAKE) -C ../mapleall ARM=1

maplearm2:
	$(MAKE) -C ../mapleall arm

mozjs:
	$(MAKE) -C ../mozjs

src:
	$(MAKE) -C $@

regression:
	$(MAKE) -C tests regression

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

checkin:
	$(MAKE) -C ../js2mpl-vm cleanall
	$(MAKE) -C ../js2mpl-vm build
	$(MAKE) -C ../dex2mpl cleanall
	$(MAKE) -C ../dex2mpl build
	$(MAKE) -C ../js2mpl-vm regression
	$(MAKE) -C ../dex2mpl regression

.PHONY: $(TARGS)

