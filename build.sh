#!/usr/bin/env bash

mkdir -p ./build

printf "\n====== Build BEKcracker ======\n"
cd src_BEKcracker && make clean && make
cd ..
mv src_BEKcracker/BEKcracker build 2> /dev/null

printf "\n====== Executables in build directory ======\n"
