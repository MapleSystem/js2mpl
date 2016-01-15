include ../Makefile.in

JS2MPLDIR = $(CURDIR)/..
MAPLEROOT = $(JS2MPLDIR)/..
MAPLEALL = $(MAPLEROOT)/mapleall

BUILD = $(CURDIR)/output
build = output

JS2MPL=$(JS2MPLDIR)/build/js2mpl
BE = $(MAPLEALL)/build/maplebe/be/mplbe
MAPLEINTERPRETER=$(MAPLEALL)/build/maplevm/interpreter32
GEN_CMPL=$(MAPLEALL)/build/maplevm/binmir/gencmpl
PRINT_CMPL=$(MAPLEALL)/build/maplevm/binmir/printcmpl
VM_CMPL=$(MAPLEALL)/build/maplevm/compact/jsvm-cmpl
MEM_PROFILER_CMPL=./get-mem-usage.sh
js2mpl=../build/js2mpl
be = ../../mapleall/build/maplebe/be/mplbe
mapleinterpreter=../../mapleall/build/maplevm/interpreter32

SUBDIR:=$(wildcard todo-tests/* todo-tests/*/* regression-tests/* regression-tests/*/*)
MODULES=$(subst temp,,$(SUBDIR))
SRCDIR:= $(MODULES)
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
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	$(JS2MPL) $(BUILD)/$*.js $(OPT)
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	@echo gdb --args $(be) $(build)/$*.mpl
	$(BE) $(BUILD)/$*.mpl > $(BUILD)/$*.log
	@echo convert mmpl to cmpl
	$(GEN_CMPL) $(BUILD)/$*.mmpl
	#@echo test cmpl print
	#$(PRINT_CMPL) $(BUILD)/$*.cmpl
	@echo gdb --args $(mapleinterpreter) $(build)/$*.mmpl
	$(MAPLEINTERPRETER) $(BUILD)/$*.mmpl
	@echo gdb --args $(mapleinterpreter) $(build)/$*.mmpl
	@echo to debug compact-vm, set DEBUG=1 when "make compact-vm"
	@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	$(VM_CMPL) $(BUILD)/$*.cmpl
	@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	@echo profile memory usage of compact-vm for the app
	$(MEM_PROFILER_CMPL) $(BUILD)/$*.cmpl 
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
