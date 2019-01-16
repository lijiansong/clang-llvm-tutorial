#!/bin/bash
PROJ=live-var
TEST_SUITE=benchmark
if [ ! -f ${PROJ} ]
then
    echo Please build ${PROJ}
    exit
fi
for ir in `ls ${TEST_SUITE}/*.ll`
do
    if [ -f ${ir} ]
    then
        echo ${ir}
        ./${PROJ} ${ir}
    fi
done
