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
	$(MAKE) -C tests regression OPT=1 IPA=1

todo:
	$(MAKE) -C tests todo

clean: 
	rm -rf build build_cmake
	rm -rf tests/output

rebuild: clean all

checkin: cleanall buildall checkall
checkinme: cleanall buildall checkallme

doit: buildall checkall

cleanall:
	$(MAKE) -C ../mapleall clean
	$(MAKE) -C ../js2mpl clean
	$(MAKE) -C ../dex2mpl clean

buildall:
	$(MAKE) -C ../mapleall gnubuild
	$(MAKE) -C ../js2mpl src
	$(MAKE) -C ../mapleall clangbuild
	$(MAKE) -C ../dex2mpl src

checkall:
	$(MAKE) -C ../js2mpl regression
	$(MAKE) -C ../dex2mpl regression

checkallme:
	$(MAKE) -C ../js2mpl regression OPT=1
	$(MAKE) -C ../dex2mpl regression OPT=3

localcheckin:
	$(MAKE) -C ../mapleall clean
	$(MAKE) -C ../js2mpl clean
	$(MAKE) -C ../mapleall gnubuild
	$(MAKE) -C ../js2mpl src
	$(MAKE) -C ../js2mpl regression

.PHONY: $(TARGS)

