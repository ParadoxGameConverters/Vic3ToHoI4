#!/bin/bash

rm -rf build/CMakeFiles
rm -rf build/gcoverage
rm -rf build/lcoverage

cmake --preset x64-coverage-linux
cmake --build --preset build-x64-coverage-linux --target Vic3ToHoi4 Vic3ToHoi4Tests -- -j40

cd build/test/Coverage-Linux || exit
./Vic3ToHoi4Tests
cd ../../.. || exit

cmake --build --preset build-x64-coverage-linux --target gcov
cmake --build --preset build-x64-coverage-linux --target lcov