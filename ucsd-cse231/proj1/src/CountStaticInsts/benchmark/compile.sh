#!/bin/bash
######################################
# LLVM ir assembly file generation.
######################################

set -e
#clang -O0 -emit-llvm -c sum.c -o sum.bc
#llvm-dis sum.bc
#clang -O0 -S -emit-llvm sum.c
for file in `ls *.c`
do
    echo compiling ${file}...
    clang -O0 -S -emit-llvm ${file}
done
echo DONE!
