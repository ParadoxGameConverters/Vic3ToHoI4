#!/bin/bash
# to be run from the project root directory
cd external/Fronter || exit
./build_linux.sh
cp -r Release/* ../../build/Release-Linux
