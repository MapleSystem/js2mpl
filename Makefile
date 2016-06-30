include Makefile.in

TARGS = mapleall mozjs src tests
TARGS = mapleall src

all: $(TARGS) regression
build: mapleall src
arm: maplearm src
arm2: maplearm2 src
clang: mapleallclang src

mapleall:
	$(MAKE) -C ../mapleall gnubuild

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
	$(MAKE) -C tests regression OPT=1

todo:
	$(MAKE) -C tests todo

clean: 
	rm -rf build

rebuild: clean all

cleanall:
	$(MAKE) -C ../mapleall clean
	$(MAKE) -C ../js2mpl-vm clean
	$(MAKE) -C ../dex2mpl clean

buildall:
	$(MAKE) -C ../mapleall gnubuild
	$(MAKE) -C ../js2mpl-vm src
	$(MAKE) -C ../mapleall clangbuild
	$(MAKE) -C ../dex2mpl src

checkin: cleanall buildall
	$(MAKE) -C ../js2mpl-vm regression
	$(MAKE) -C ../dex2mpl regression

localcheckin:
	$(MAKE) -C ../mapleall clean
	$(MAKE) -C ../js2mpl-vm clean
	$(MAKE) -C ../mapleall gnubuild
	$(MAKE) -C ../js2mpl-vm src
	$(MAKE) -C ../js2mpl-vm regression

.PHONY: $(TARGS)

