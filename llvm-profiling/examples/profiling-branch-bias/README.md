## Profiling Branch Bias
Count the number of times conditional branch instructions are executed and the number of times conditional branch instructions are taken. Note that we only consider `conditional branches`.
A conditional branch is considered taken if its condition evaluates to true. Each instrumented function should output these two counts before function termination.

## Build

```
$ make
$ ./pbb benchmark/sum.ll
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
taken	2048
total	2050
taken	0
total	0
```

Notice: Current implementation based on LLVM version 5.0.0

## REF
- CSE231 project part1: <https://ucsd-pl.github.io/cse231/wi17/part1.html>
