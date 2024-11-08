#!/bin/bash
cmake --preset x64-release-linux
cmake --build --preset build-x64-release-linux --target Vic3ToHoi4 ConverterFrontend Vic3ToHoi4Tests -- -j40
(
cd build/test/Release-Linux || exit
./Vic3ToHoi4Tests
)