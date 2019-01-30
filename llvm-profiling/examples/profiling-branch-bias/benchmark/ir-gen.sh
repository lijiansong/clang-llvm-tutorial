#!/bin/bash
######################################
# LLVM ir assembly file generation.
######################################

set -e
#clang -O0 -emit-llvm -c sum.c -o sum.bc
#llvm-dis sum.bc
#clang -O0 -S -emit-llvm sum.c
rm -f *.o
for file in `ls *.c`
do
    echo compiling ${file}...
    #clang -g3 -O0 -S -emit-llvm ${file}
    prefix=`basename ${file} .c`
    #clang -O3 -S -emit-llvm ${file} -o ${prefix}.ll
    clang -S -emit-llvm ${file} -o ${prefix}.ll
    # Assign names to anonymous instructions
    #opt -instnamer ${prefix}.ll -o ${prefix}.ll
    #opt -mem2reg ${prefix}.ll -o ${prefix}.ll
    #llvm-dis ${prefix}.ll -o ${prefix}.ll
done
echo DONE!
