#!/bin/bash

PREFIX=/Users/ilan/a/envs/py37

if [ `uname` == Darwin ]; then
    gcc -Wall -arch x86_64 \
          -I${PREFIX}/include/python3.7m -c main.c -o main.o
    gcc -undefined dynamic_lookup -arch x86_64 \
          -L${PREFIX}/lib -lpython3.7m main.o -o wfc
    install_name_tool -change @rpath/libpython3.7m.dylib \
                      ${PREFIX}/lib/libpython3.7m.dylib ./wfc
fi
