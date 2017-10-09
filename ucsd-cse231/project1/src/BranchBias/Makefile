PROJECT=branch-bias
COMMON_FLAGS=-Wall -Wextra -g
SRC_DIR?=$(PWD)
SRC=BranchBias.cpp
all:
	@echo Compiling $(SRC)
	clang++ -g -O3 $(SRC_DIR)/$(SRC) $(COMMON_FLAGS) -I$(SRC_DIR) `llvm-config --cxxflags --ldflags --system-libs --libs irreader bitwriter passes core support` -fno-rtti -o $(PROJECT)
clean:
	rm -f $(PROJECT)
