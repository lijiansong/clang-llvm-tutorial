TARGET=dbg-info
all:
	@clang++ -g -O3 dbg_info.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core executionengine object support native` -o $(TARGET)
clean:
	@rm -f $(TARGET)
