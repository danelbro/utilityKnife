#! /bin/bash

cmake --build --preset $1 -j$((`nproc`+1))
