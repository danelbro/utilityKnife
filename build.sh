#! /bin/bash

cmake --build out/build/ -j$((`nproc`+1))
