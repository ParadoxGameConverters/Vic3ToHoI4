#!/bin/bash
cmake -H. -Bbuild -DCOVERAGE=TRUE && cmake --build ./build -- -j40
cd build || exit
cd test/Release-Linux || exit
./Vic3ToHoi4Tests
cd ../.. || exit
make gcov
make lcov