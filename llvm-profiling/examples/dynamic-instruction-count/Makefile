PROJECT=cdi
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    DBG_FILES := ${PROJECT}.dSYM
endif
ifeq ($(UNAME_S),Linux)
    DBG_FILES :=
endif

COMMON_FLAGS=-Wall -Wextra #-g
SRC_DIR?=$(PWD)
SRC=CountDynamicInstructions.cpp

all:
	@echo Compiling $(SRC)
	clang++ $(SRC) $(COMMON_FLAGS) `llvm-config --cxxflags --ldflags --system-libs --libs irreader bitwriter passes core support` -O3 -o $(PROJECT)
clean:
	rm -rf $(PROJECT) $(DBG_FILES) foo a.out
