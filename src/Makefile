#
# Copyright (C) [2021] Futurewei Technologies, Inc. All rights reserved.
#
# OpenArkCompiler is licensed under the Mulan Permissive Software License v2.
# You can use this software according to the terms and conditions of the MulanPSL - 2.0.
# You may obtain a copy of MulanPSL - 2.0 at:
#
#   https://opensource.org/licenses/MulanPSL-2.0
#
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the MulanPSL - 2.0 for more details.
#
include ../Makefile.in

EXE=js2mpl

JS2MPLDIR = $(MAPLEROOT)/js2mpl
MAPLEALL = $(MAPLEROOT)/mapleall
BUILD=$(JS2MPLDIR)/build/$(FLAVOR)

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(SRC:.cpp=.d)

OBJS :=$(foreach obj, $(OBJ), $(BUILD)/$(obj))
DEPS :=$(foreach dep, $(DEP), $(BUILD)/$(dep))

MOZSRC=$(MAPLEROOT)/mozjs/mozjs-31.2.0
MOZBUILD=$(MAPLEROOT)/mozjs/mozjs-build
MAPLELIB=$(MAPLEALL)/build/$(FLAVOR)/mapleir/libmpl.a
MPLIB=$(MAPLEALL)/build/$(FLAVOR)/Mempool/libmempool.a
#LIBS=-L $(MOZBUILD)/dist/lib -ljs_static -lpthread -ldl
LIBS=-L $(MOZBUILD)/dist/lib -ljs_static -lz -lpthread -ldl

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
	$(LD) $(CXXFLAGS) -o $(BUILD)/$(EXE) $(OBJS) $(MAPLELIB) $(MPLIB) $(LIBS)

.cpp.o: $(BUILD)/$*.d
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(INCLUDES) -w -c $(CURDIR)/$*.cpp -o $(BUILD)/$*.o
	@$(CXX) -std=c++11 -MM $(INCLUDES) $(CURDIR)/$*.cpp > $(BUILD)/$*.d

clean:
	rm -rf $(BUILD)
