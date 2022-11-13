#!/bin/bash
rm build/test/Release-Linux/Vic3ToHoi4Tests
cmake -H. -Bbuild && cmake --build ./build --target Vic3ToHoi4 Vic3ToHoi4Tests -- -j40
(
cd build/test/Release-Linux || exit
./Vic3ToHoi4Tests
)