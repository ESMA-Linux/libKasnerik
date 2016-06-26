#!/bin/bash

rm -rf ./build_mingw/
mkdir -p ./build_mingw/
cd ./build_mingw/

cmake ../.. -DCMAKE_TOOLCHAIN_FILE="../toolchains/MingwOnLinux.toolchain"
make
make install
