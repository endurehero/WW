#!/bin/bash

if [ -d "./ww_build" ]; then
    rm -rf ./ww_build
fi

if [ -d "./output" ]; then
    rm -rf ./output
fi

mkdir ./ww_build
cd ./ww_build

cmake \
    -DENABLE_DEBUG=NO \
    -DUSE_CPU=YES \
    -DUSE_GPU=NO \
        -DUSE_CUDA=YES \
        -DUSE_HIP=NO \
        -DUSE_OCL=NO \
    -DBUILD_WITH_UNIT_TEST=YES \
    ..
    
make