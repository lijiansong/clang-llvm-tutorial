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
#Add the static libs that may be linked
LLVMLIBS=$(shell $(LLVM_CONFIG) --libs irreader bitwriter passes core support)
SYSTEMLIBS=$(shell $(LLVM_CONFIG) --system-libs)

PROJECT=llvm-assignment
PROJECT_OBJECTS=LLVMAssignment.o

default: $(PROJECT)

#%.o : $(SRC_DIR)/%.cpp
$(PROJECT_OBJECTS) : $(SRC_DIR)/LLVMAssignment.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PROJECT) : $(PROJECT_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LLVMLIBS) $(SYSTEMLIBS)

clean::
	$(QUIET)rm -f $(PROJECT) $(PROJECT_OBJECTS)
