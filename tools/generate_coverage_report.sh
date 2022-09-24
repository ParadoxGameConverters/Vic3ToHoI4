#!/bin/bash
rm -rf build/
cmake -H. -Bbuild -DCOVERAGE=TRUE && cmake --build ./build --target Vic3ToHoi4 Vic3ToHoi4Tests -- -j40
cd build || exit
cd test/Release-Linux || exit
./Vic3ToHoi4Tests
cd ../.. || exit
make gcov
make lcov