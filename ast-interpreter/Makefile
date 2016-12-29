# Copyright (c) 2016 Json Lee. All rights reserved.
# For more info, see [here](https://github.com/lijiansong) or my [github page](https://lijiansong.github.io/)

LLVM_CONFIG?=llvm-config

ifndef VERBOSE
QUIET:=@
endif

SRC_DIR?=$(PWD)
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)

#Debug flags
COMMON_FLAGS=-Wall -Wextra -g
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags) -fno-rtti
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR)
# include all dependency clang libs
CLANGLIBS=\
  -Wl,--start-group\
  -lclang\
  -lclangFrontend\
  -lclangDriver\
  -lclangSerialization\
  -lclangParse\
  -lclangSema\
  -lclangAnalysis\
  -lclangEdit\
  -lclangAST\
  -lclangLex\
  -lclangBasic\
  -lclangTooling\
  -Wl,--end-group
LLVMLIBS=$(shell $(LLVM_CONFIG) --libs)
SYSTEMLIBS=$(shell $(LLVM_CONFIG) --system-libs)

PROJECT=ast-interpreter
PROJECT_OBJECTS=ASTInterpreter.o

default: $(PROJECT)

# rule to build object files
%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PROJECT) : $(PROJECT_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(CLANGLIBS) $(LLVMLIBS) $(SYSTEMLIBS)

clean::
	$(QUIET)rm -f $(PROJECT) $(PROJECT_OBJECTS)
