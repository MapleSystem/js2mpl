include ../Makefile.in

JS2MPLDIR = $(CURDIR)/..
MAPLEROOT = $(JS2MPLDIR)/..
MAPLEALL = $(MAPLEROOT)/mapleall

BUILD = $(CURDIR)/output

JS2MPL=$(JS2MPLDIR)/build/js2mpl
BE = $(MAPLEALL)/build/maplebe/be/mplbe
MAPLEINTERPRETER=$(MAPLEALL)/build/maplevm/interpreter32

SUBDIR:=$(wildcard regression-tests/* todo-tests/*)
MODULES=$(subst temp,,$(SUBDIR))
SRCDIR:= todo-tests $(MODULES)
SRC0=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.js))
SRC=$(notdir $(SRC0))

MPL=$(SRC:.js=.mpl)
MMPL=$(SRC:.js=.mmpl)
CASE=$(SRC:.js=)

all: DIRECTORIES
	@echo "=================== usage ========================="
	@echo "= test single case:     make foo will test foo.js ="
	@echo "==================================================="

genmpl: $(MPL)
genmmpl: $(MMPL)
case: $(CASE)

.PHONY: clean
vpath %.js $(SRCDIR)
vpath %.mpl $(BUILD)
vpath %.mmpl $(BUILD)

DIRECTORIES:
	$(MKDIR_P) $(BUILD)

% : %.js $(JS2MPL) $(BE) $(MAPLEINTERPRETER) DIRECTORIES
	cp $< $(BUILD)/$*.js
	$(JS2MPL) $(BUILD)/$*.js $(OPT)
	$(BE) $(BUILD)/$*.mpl > $(BUILD)/$*.log
	$(MAPLEINTERPRETER) $(BUILD)/$*.mmpl
	@echo ""

src:
	$(MAKE) -C ../$@

todo: run

run:
	@./runtests.pl todo-tests

checkin: regression

regression:
	@./runtests.pl regression-tests

clean:
	rm -f $(BUILD)/*
