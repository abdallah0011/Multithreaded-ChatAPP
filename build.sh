#!/bin/bash

#clean
echo "============CLEANING============"
rm -rf build
rm -rf run
echo "==============DONE=============="

#build
echo "============BUILDING============"
mkdir build
cd build || { echo "Failed to locate build directory"; exit 1; }

cmake .. && make -j
cmake --install .
