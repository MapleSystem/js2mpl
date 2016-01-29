include ../Makefile.in

JS2MPLDIR = $(CURDIR)/..
MAPLEROOT = $(JS2MPLDIR)/..
MAPLEALL = $(MAPLEROOT)/mapleall

BUILD = $(CURDIR)/output
REQUIRED = $(CURDIR)/output/required
build = output
required = output/required

JS2MPL=$(JS2MPLDIR)/build/js2mpl
BE = $(MAPLEALL)/build/maplebe/be/mplbe
MAPLEINTERPRETER=$(MAPLEALL)/build/maplevm/interpreter32
GEN_CMPL=$(MAPLEALL)/build/maplevm/cmpl-v1/gencmpl-v1
PRINT_CMPL=$(MAPLEALL)/build/maplevm/cmpl-v1/printcmpl-v1
VM_CMPL=$(MAPLEALL)/build/maplevm/compact/jsvm-cmpl-v1
GEN_CMPL_V2=$(MAPLEALL)/build/maplevm/cmpl/gencmpl
PRINT_CMPL_V2=$(MAPLEALL)/build/maplevm/cmpl/printcmpl
MEM_PROFILER_CMPL=./get-mem-usage.sh
js2mpl=../build/js2mpl
be = ../../mapleall/build/maplebe/be/mplbe
mapleinterpreter=../../mapleall/build/maplevm/interpreter32

PLUGINDIR=todo-tests/plugin regression-tests/plugin
REQDIR=todo-tests/plugin/required regression-tests/plugin/required
PLUGINREQDIR=$(PLUGINDIR) $(REQDIR)
PLUGINSRC0=$(foreach sdir, $(PLUGINDIR), $(wildcard $(sdir)/*.js))
REQSRC0=$(foreach sdir, $(REQDIR), $(wildcard $(sdir)/*.js))
PLUGINSRC=$(notdir $(PLUGINSRC0))
REQSRC=$(notdir $(REQSRC0))

#PLUGIN=$(PLUGINSRC:.js=.p)
REQ=$(REQSRC:.js=.r)

SUBDIR0:=$(wildcard todo-tests/* todo-tests/*/* regression-tests/* regression-tests/*/*)
SUBDIR=$(filter-out $(PLUGINREQDIR),$(SUBDIR0))
MODULES=$(subst temp,,$(SUBDIR))
SRCDIR:= $(MODULES)
SRC0=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.js))
SRC=$(notdir $(SRC0))

MPL=$(SRC:.js=.mpl)
MMPL=$(SRC:.js=.mmpl)
CASE=$(SRC:.js=)

all: DIRECTORIES
	@echo "=================== usage =============================="
	@echo "= test single case: make foo will test foo.js          ="
	@echo "=                   make foo.p will test plugin foo.js ="
	@echo "========================================================"

.PHONY: clean
vpath %.js $(SRCDIR) $(PLUGINDIR) $(REQDIR)
vpath %.mpl $(BUILD)
vpath %.mmpl $(BUILD)
vpath %.p $(BUILD)
vpath %.r $(REQUIRED)

DIRECTORIES:
	$(MKDIR_P) $(BUILD)/required

tt:
	echo $(SRCDIR)
	echo $(SRC)
	echo $(PLUGINDIR)
	echo $(PLUGINSRC)
	echo $(REQDIR)
	echo $(REQSRC)
	echo $(REQ)

% : %.js $(JS2MPL) $(BE) $(MAPLEINTERPRETER) DIRECTORIES
	cp $< $(BUILD)/$*.js
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	$(JS2MPL) $(BUILD)/$*.js $(OPT)
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	@echo gdb --args $(be) $(build)/$*.mpl
	$(BE) $(BUILD)/$*.mpl > $(BUILD)/$*.log
	@echo convert mmpl to cmpl
	$(GEN_CMPL) $(BUILD)/$*.mmpl
	cp $(BUILD)/$*.cmpl $(BUILD)/$*.cmpl.v1
	@#@echo test cmpl print
	@#$(PRINT_CMPL) $(BUILD)/$*.cmpl
	@echo gdb --args $(mapleinterpreter) $(build)/$*.mmpl
	$(MAPLEINTERPRETER) $(BUILD)/$*.mmpl
	@echo gdb --args $(mapleinterpreter) $(build)/$*.mmpl
	@echo to debug compact-vm, set DEBUG=1 when "make compact-vm"
	@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	$(VM_CMPL) $(BUILD)/$*.cmpl
	@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	@echo profile memory usage of compact-vm for the app
	$(MEM_PROFILER_CMPL) $(BUILD)/$*.cmpl 
	@echo convert mmpl to cmpl-v2
	$(GEN_CMPL_V2) $(BUILD)/$*.mmpl
	cp $(BUILD)/$*.cmpl $(BUILD)/$*.cmpl.v2
	@echo ""

%.p : %.js $(REQ) $(JS2MPL) $(BE) $(MAPLEINTERPRETER) DIRECTORIES
	cp $< $(BUILD)/$*.js
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	$(JS2MPL) $(BUILD)/$*.js $(OPT)
	@echo gdb --args $(js2mpl) $(build)/$*.js $(OPT)
	@echo gdb --args $(be) $(build)/$*.mpl
	$(BE) $(BUILD)/$*.mpl > $(BUILD)/$*.log
	@echo convert mmpl to cmpl
	$(GEN_CMPL) $(BUILD)/$*.mmpl
	cp $(BUILD)/$*.cmpl $(BUILD)/$*.cmpl.v1
	@#@echo test cmpl print
	@#$(PRINT_CMPL) $(BUILD)/$*.cmpl
	@echo gdb --args $(mapleinterpreter) $*.mmpl
	(cd $(BUILD); $(MAPLEINTERPRETER) $*.mmpl)
	@echo gdb --args $(mapleinterpreter) $*.mmpl
	@#@echo to debug compact-vm, set DEBUG=1 when "make compact-vm"
	@#@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	@#$(VM_CMPL) $(BUILD)/$*.cmpl
	@#@echo gdb --args $(VM_CMPL) $(build)/$*.cmpl
	@#@echo profile memory usage of compact-vm for the app
	@#$(MEM_PROFILER_CMPL) $(BUILD)/$*.cmpl 
	@#@echo convert mmpl to cmpl-v2
	@#$(GEN_CMPL_V2) $(BUILD)/$*.mmpl
	@#cp $(BUILD)/$*.cmpl $(BUILD)/$*.cmpl.v2
	@echo ""

%.r : %.js $(JS2MPL) $(BE) DIRECTORIES
	cp $< $(REQUIRED)/$*.js
	@echo gdb --args $(js2mpl) $(required)/$*.js -plungin
	$(JS2MPL) $(required)/$*.js -plugin
	@echo gdb --args $(js2mpl) $(required)/$*.js -plungin
	@echo gdb --args $(be) $(required)/$*.mpl
	$(BE) $(REQUIRED)/$*.mpl > $(REQUIRED)/$*.log
	@echo convert mmpl to cmpl
	$(GEN_CMPL) $(REQUIRED)/$*.mmpl
	@#cp $(REQUIRED)/$*.cmpl $(REQUIRED)/$*.cmpl.v1
	@#@echo test cmpl print
	@#$(PRINT_CMPL) $(BUILD)/$*.cmpl
	@#@echo gdb --args $(mapleinterpreter) $(required)/$*.mmpl
	@#$(MAPLEINTERPRETER) $(REQUIRED)/$*.mmpl
	@#@echo gdb --args $(mapleinterpreter) $(required)/$*.mmpl
	@#@echo to debug compact-vm, set DEBUG=1 when "make compact-vm"
	@#@echo gdb --args $(VM_CMPL) $(required)/$*.cmpl
	@#$(VM_CMPL) $(REQUIRED)/$*.cmpl
	@#@echo gdb --args $(VM_CMPL) $(required)/$*.cmpl
	@#@echo profile memory usage of compact-vm for the app
	@#$(MEM_PROFILER_CMPL) $(REQUIRED)/$*.cmpl 
	@#@echo convert mmpl to cmpl-v2
	@#$(GEN_CMPL_V2) $(REQUIRED)/$*.mmpl
	@#cp $(REQUIRED)/$*.cmpl $(REQUIRED)/$*.cmpl.v2
	@cp $(REQUIRED)/$*.mmpl $(REQUIRED)/$*.r
	@echo ""

src:
	$(MAKE) -C ../$@

todo: run

run:
	@./runtests.pl todo-tests $(OPT)

plugin: $(REQ)

checkin: regression

regression:
	@./runtests.pl regression-tests $(OPT)

clean:
	rm -f $(BUILD)/*
