# Clang and LLVM Tutorial

This archieve is mainly about some interesting examples of clang and llvm... Some of them  are the homework of my postgraduate course, some of them are others, most of them are `toy code` : )

## Contents
* [AST Interpreter](#ast-interpreter)
* [Function Pointer Pass](#function-pointer-pass)
* [Value Range Analysis](#value-range-analysis)
* [Data-Flow Analysis](#data-flow-analysis)
* [Andersen Pointer Analysis](#andersen-pointer-analysis)
* [LLVM Backend](#llvm-backend)

### AST Interpreter
An interpreter of clang abstract syntax tree, toy code, now it only support integer type. It supports basic modules of programming language, e.g. ifstmt, whilestmt, function call, malloc and array... For more info, see [here...](https://github.com/lijiansong/clang-llvm-tutorial/tree/master/ast-interpreter)

### Function Pointer Pass
Implement of use-def chains based on the LLVM IR and bitcode, now it supports direct function calls and function pointer. For the case of function pointer, calculate the functions that may call, if it is determined, replace it with direct function call and write into the bitcode file. Also take the situation that the function pointers are stored in memory into consideration.

### Value Range Analysis
`Value range analysis` is a type of `data flow analysis` that tracks the range (interval) of values that a numeric variable can take on at each point of a program's execution. <br>
The resulting information can be used in optimizations such as `redundancy elimination, dead code elimination, instruction selection, etc`. But can also be used to improve the `safety of programs`, e.g. in the detection of buffer overruns. 
Techniques for value range analysis typically use `symbolic analysis` extensively. When it comes to the detailed implement, for each basic block, define the following data-flow equation：
```
in[B] = ∪ out[A] | A∈ pred(B) && A！=S
out[B] = in[B]∪gen[B]
```
### Data Flow Analysis
Homework of foreign CS course --- `Advanced Compilers`, for more detail, see [here...](http://cseweb.ucsd.edu/classes/sp11/cse231-a/)

### Andersen Pointer Analysis
Implement of Andersen's pointer analysis algorithm, for more info, see [here...](https://github.com/lijiansong/clang-llvm-tutorial/tree/master/andersen-pointer-analysis)

### LLVM Backend
For a compiling system, I think the most complex part is the back end, you have to be proficient in instruction sets of the target processor and familiar with its architecture. Lots of things have to be done, e.g. instruction selection, instruction scheduling, register allocation, code emission and etc. Here i will provide you will an example, for more details, see [here](./llvm-backend)



