#!/bin/bash


if [ `uname` == Darwin ]; then
    PYTHON_PREFIX=/Users/ilan/a/envs/py37

    gcc -Wall -arch x86_64 \
        -I${PYTHON_PREFIX}/include/python3.7m -c main.c -o main.o
    gcc -undefined dynamic_lookup -arch x86_64 \
        -L${PYTHON_PREFIX}/lib -lpython3.7m main.o -o wfc
    install_name_tool -change @rpath/libpython3.7m.dylib \
                      ${PYTHON_PREFIX}/lib/libpython3.7m.dylib ./wfc
fi
