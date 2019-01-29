## Collecting Dynamic Instruction Counts
At a high level, this pass obtains dynamic instruction counts by inserting an instrumentation module into the program at compile time.
At the end of every basic block, the LLVM pass inserts calls to the instrumentation module, which is subsequently linked to the original executable.These calls record the executed instructions within the current basic block. When the program is run, it prints out the dynamic instruction counts just before the main function terminates.

The implementation is that we go through every module, every function and every basic block. For each basic block, we statically create a C++ map, which keeps track of the instructions in the basic block as well as their counts. Next, we find the terminator of this block, e.g.: a ret or jump instruction.
However, given that entering a basic block implies that every instruction within it `must` be executed, we can significantly reduce the number of calls to the external statistics-collection code. This can be done by aggregating the instruction counts per basic block at compile time, and recording them once before exiting a basic block.

## Build

```
$ make
$ ./cdi benchmark/sum.ll
```

then you will get an output of the bc file after instrumentation which maybe unreadable, you can use `llvm-dis` to see the instrumented IR.
Now we can link the instrumented ir with the runtime library `runtime.cpp` by the following commands:

```
$ llc -filetype=obj benchmark/sum.ll
$ clang++ benchmark/sum.o runtime.cpp -o foo
$ ./foo
```

Now we can get the following output:

```
ret	1
br	6148
add	5120
alloca	10
load	19459
store	6151
getelementptr	6144
sext	6144
icmp	2050
ret	1
getelementptr	3
call	1
```

Notice: Current implementation based on LLVM version 5.0.0

## REF
- CSE231 project part1: <https://ucsd-pl.github.io/cse231/wi17/part1.html>
