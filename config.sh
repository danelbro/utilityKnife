#! /bin/bash

mkdir -p out/build/
cd out/build
cmake -DBUILD_SHARED_LIBS=OFF -DSDL_TEST_LIBRARY=OFF -DSDLTTF_VENDORED=ON ../../
