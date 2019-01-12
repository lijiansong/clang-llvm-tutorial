PROJECT=count-static-insts
COMMON_FLAGS=-Wall -Wextra -g
SRC_DIR?=$(PWD)
SRC=CountStaticInstructions.cpp
all:
	@echo Compiling $(SRC)
	clang++ $(SRC) $(COMMON_FLAGS) `llvm-config --cxxflags --ldflags --system-libs --libs irreader bitwriter passes core support` -O3 -o $(PROJECT)
clean:
	rm -rf $(PROJECT) ${PROJECT}.dSYM
