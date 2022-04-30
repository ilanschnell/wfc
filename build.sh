#!/bin/bash

if [ `uname` == Darwin ]; then
    PYTHON_PREFIX=/Users/ilan/Mini3/envs/py311

    gcc -Wall -arch x86_64 \
        -I${PYTHON_PREFIX}/include/python3.11 -c main.c -o main.o
    gcc -undefined dynamic_lookup -arch x86_64 \
        -L${PYTHON_PREFIX}/lib -lpython3.11 main.o -o wfc
    #install_name_tool -change @rpath/libpython3.8.dylib \
    #                  ${PYTHON_PREFIX}/lib/libpython3.8.dylib ./wfc
fi
if [ `uname` == Linux ]; then
    PYTHON_PREFIX=/usr

    gcc -Wall -I${PYTHON_PREFIX}/include/python3.7m -c main.c -o main.o
    gcc -L${PYTHON_PREFIX}/lib -lpython3.7m main.o -o wfc
fi
