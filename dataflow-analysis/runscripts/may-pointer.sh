#!/bin/bash

filename=$(basename "$1")
opt -analyze -load $LLVMLIB/CSE231.so -instnamer -may_pointer < $1/$filename.bc > /dev/null
