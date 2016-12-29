#!/bin/bash

filename=$(basename "$1")
opt -mem2reg -analyze -load $LLVMLIB/CSE231.so -instnamer -avail_expr < $1/$filename.bc > /dev/null
#opt -analyze -load $LLVMLIB/CSE231.so -instnamer -avail_expr < $1/$filename.bc > /dev/null


