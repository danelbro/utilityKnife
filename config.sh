#! /bin/bash

mkdir -p build/$1
cmake --preset $1
mv build/$1/compile_commands.json .
