TARGET=target-triple
all:
	clang++ -g -O3 target-triple.cpp `llvm-config --cxxflags --ldflags --system-libs --libs all` -o $(TARGET)
clean:
	@rm -f $(TARGET)
