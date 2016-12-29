##FuncPtrPass

Toy code about use-def chains based on the LLVM IR and bitcode. Output the function call info in the bitcode file, e.g.
- Direct function calls. Print the function name and line no.
- FuncPtr i.e. function pointer. Calculate the functions that may call, if it is determined, replace it with direct function call. Consider the situation that the function pointers are stored in the memory.

## Build

```
$ cd your-directory
$ make
$ ./llvm-assignment your-bitcode-file
```

LLVM IR has three equivalent forms:
- An in-memory representation(the Instruction class, among others)
- An on-disk representation that is encoded in a space-efficient form(the bitcodefiles)
- An on-disk representation in a human-readable text form(the LLVM assembly files)

## Gen bitcode
- bitcode generation using following cmds
```
$ clang -c -emit-llvm -g3 funcptr.c
$ opt -dot-cfg-only funcptr.bc
$ opt -S –mem2reg funcptr.bc –o funcptr.opt
```

## Postscripts
- Remove every uses of [getGlobalContext](https://reviews.llvm.org/rL266379), but the C API, and the [solution...](https://reviews.llvm.org/D19094)
