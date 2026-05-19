#! /bin/bash

mkdir -p build/$1
cmake -S . -Bbuild/$1 -GNinja -DCMAKE_BUILD_TYPE=$1
mv build/$1/compile_commands.json .
