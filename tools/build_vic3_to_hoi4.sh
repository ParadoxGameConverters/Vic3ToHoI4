#!/bin/bash
cmake -H. -Bbuild && \ 
cmake --build ./build --target Vic3ToHoi4 ConverterFrontend Vic3ToHoi4Tests -- -j40 && \
cd build/test/Release-Linux && \
./Vic3ToHoi4Tests && \
cd ..
