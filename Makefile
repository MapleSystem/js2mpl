include Makefile.in

ifndef OPT
	OPT = 0
endif

TARGS = src tests

all: $(TARGS)
build: src
test: tests

src:
	$(MAKE) -C $@

tests:
	$(MAKE) -C $@ regression OPT=$(OPT)

clean: 
	$(MAKE) clean -C src
	$(MAKE) clean -C tests

.PHONY: $(TARGS)

