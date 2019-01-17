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
    #clang -g3 -O0 -S -emit-llvm ${file}
    prefix=`basename ${file} .c`
    clang -S -emit-llvm ${file} -o ${prefix}.ll
    # Assign names to anonymous instructions
    opt -instnamer ${prefix}.ll -o ${prefix}.ll
    llvm-dis ${prefix}.ll -o ${prefix}.ll
done
echo DONE!
