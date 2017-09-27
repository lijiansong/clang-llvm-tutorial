TARGET=jit
all:
	clang++ -g -O3 jit.cpp `llvm-config --cxxflags --ldflags --system-libs --libs coremc jit native` -o $(TARGET)
clean:
	rm -f $(TARGET)
