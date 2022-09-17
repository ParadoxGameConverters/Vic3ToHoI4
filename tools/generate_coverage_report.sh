#!/bin/bash
cmake -H. -Bbuild && cmake --build ./build -- -j40
cd build
cd test/Release-Linux
./Vic3ToHoi4Tests
cd ../..
make gcov
make lcov