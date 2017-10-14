# !/bin/bash
llvm-tblgen -print-records insns.td 2>&1 | tee result.txt

