##AST Interpreter

Toy code, an AST interpreter, based on Calng & LLVM, that only support integer type.
It supports basic modules and array, e.g. ifstmt, whilestmt, function call, malloc and array.
It can run normally for the following test cases: test-if.c test-while.c test-fn.c test-malloc.c and test-array.c that are in the test folder.
Makefile is self-written, so the build step is easy, by following commands:

```
$ make
$ ./ast-interpreter "`cat ./test/test_file_name`"
```

If you want tp rebuild it, you can take use of `make clean` in the command line.

For more info, see [here...](https://github.com/lijiansong/Postgraduate-Course/tree/master/Compilation/Clang%20%26%20LLVM%20Tutorial/hw1)

Time limit, deref and struct haven't finished, I will complete it later...
