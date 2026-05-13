#! /bin/bash

mkdir -p build/$1
cmake -S . -Bbuild/$1 -GNinja -DCMAKE_BUILD_TYPE=$1 -DBUILD_SHARED_LIBS=OFF -DSDL_TEST_LIBRARY=OFF -DSDLTTF_VENDORED=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
mv build/$1/compile_commands.json .
