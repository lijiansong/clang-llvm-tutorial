#!/bin/bash
set -e
clang -S -emit-llvm sum.c -o sum.ll
opt -passes='print<memoryssa>' -disable-output sum.ll 2>&1 | tee mm-ssa.ll
