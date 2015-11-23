include ../Makefile.in

EXE=js2mpl

JS2MPLDIR = $(CURDIR)/..
MAPLEROOT = $(JS2MPLDIR)/..
MAPLEALL = $(MAPLEROOT)/mapleall
BUILD=$(JS2MPLDIR)/build

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(SRC:.cpp=.d)

OBJS :=$(foreach obj, $(OBJ), $(BUILD)/$(obj))
DEPS :=$(foreach dep, $(DEP), $(BUILD)/$(dep))

MOZSRC=$(MAPLEROOT)/mozjs/mozjs-31.2.0
MOZBUILD=$(MAPLEROOT)/mozjs/mozjs-build
MAPLELIB=$(MAPLEALL)/build/mapleir/libmpldev.a
LIBS=-L $(MOZBUILD)/dist/lib -ljs_static -lpthread -ldl
#LIBS=-L $(MOZBUILD)/dist/lib -ljs_static -lz -lpthread -ldl

INCLUDES = -I $(MAPLEALL) \
           -I $(MOZBUILD)/dist/include \
           -I $(MOZBUILD)/dist/system_wrappers \
           -include $(MOZSRC)/config/gcc_hidden.h  \
           -I $(MOZSRC) \
           -I $(MOZSRC)/js/src \
           -I . \
           -I $(MOZSRC)/mfbt/double-conversion \
           -I $(MOZSRC)/intl/icu/source/common \
           -I $(MOZSRC)/intl/icu/source/i18n \
           -include $(MOZBUILD)/js/src/js-confdefs.h \
           -I $(MOZSRC)/js/public

all: DIRECTORIES $(EXE)

-include $(DEPS)

.PHONY: clean

vpath %.o $(BUILD)
vpath $(EXE) $(BUILD)

DIRECTORIES:
	$(MKDIR_P) $(BUILD)

$(EXE): $(OBJ) $(MAPLELIB)
	$(CC) $(CXXFLAGS) -o $(BUILD)/$(EXE) $(OBJS) $(MAPLELIB) $(LIBS)

.cpp.o: $(BUILD)/$*.d
	$(CC) $(CXXFLAGS) $(LFLAGS) $(INCLUDES) -w -c $(CURDIR)/$*.cpp -o $(BUILD)/$*.o
	@$(CC) -std=c++11 -MM $(INCLUDES) $(CURDIR)/$*.cpp > $(BUILD)/$*.d

clean:
	rm -rf $(BUILD)
