#!/bin/bash
set -e
clang -O0 -emit-llvm -c sum.c -o sum.bc
llvm-dis sum.bc
