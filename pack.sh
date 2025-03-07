#!/bin/sh

cmake -B cmake-build -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build
tar -czvf openpgn.tar.gz cmake-build/libOpenPGN.so include
rm -r cmake-build
