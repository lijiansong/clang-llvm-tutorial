TARGET=func_prog
all:
	clang++ -g -O3 functional.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native` -o $(TARGET)
clean:
	@rm -f $(TARGET)
