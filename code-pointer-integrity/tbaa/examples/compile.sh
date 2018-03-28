#!/bin/bash
set -e
clang -cc1 -O1 -no-struct-path-tbaa -disable-llvm-optzns test.c -emit-llvm -o - 2>&1 | tee test.log
