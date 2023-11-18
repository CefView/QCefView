#!/bin/bash

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/macos.x86_64"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

echo ============== Config project ==============
cmake -G "Xcode" -S . -B "${BUILD_DIR}" -DPROJECT_ARCH=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=ON -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/macos.x86_64" $*

if [ ${BUILD_PROJECT} -eq 1 ] 
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}"
fi