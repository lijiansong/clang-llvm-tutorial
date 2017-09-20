TARGET=ir-gen
all:
	clang++ -g -O3 gen-ir.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o $(TARGET)
clean:
	rm -f $(TARGET)
