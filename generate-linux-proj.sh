#!/bin/bash

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/Linux"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Unix Makefiles" -S . -B "${BUILD_DIR}" -DBUILD_DEMO=ON -DUSE_SANDBOX=ON

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}"
fi