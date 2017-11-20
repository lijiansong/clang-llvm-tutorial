TARGET=jit
all:
	@clang++ -g -O3 jit.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native` -o $(TARGET)
clean:
	@rm -f $(TARGET)
