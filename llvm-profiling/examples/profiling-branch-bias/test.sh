#!/bin/bash
PROJ=pbb
TEST_SUITE=benchmark
RUNTIME_LIB=runtime.cpp
if [ ! -f ${PROJ} ]
then
    echo Please build ${PROJ}
    exit
fi

rm -f ${TEST_SUITE}/*.o ${TEST_SUITE}/*.ll
cd ${TEST_SUITE}
./ir-gen.sh
cd ..
for ir in `ls ${TEST_SUITE}/*.ll`
do
    if [ -f ${ir} ]
    then
        prefix=`basename ${ir} .ll`
        echo "===----- ${ir} -----==="
        ./${PROJ} ${ir}
        llvm-dis ${ir} -o ${ir}
        llc -filetype=obj ${ir}
        clang++ ${TEST_SUITE}/${prefix}.o ${RUNTIME_LIB} -o foo
        ./foo
        rm -f foo ${TEST_SUITE}/${prefix}.o
    fi
done
