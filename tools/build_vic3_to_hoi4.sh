#!/bin/bash
cmake -H. -Bbuild && cmake --build ./build --target Vic3ToHoi4 Fronter Vic3ToHoi4Tests -- -j40 && cd build/test/Release-Linux
pwd
ls
./Vic3ToHoI4Tests && cd ..
