#! /bin/bash

cmake --build build/$1 -j$((`nproc`+1))
