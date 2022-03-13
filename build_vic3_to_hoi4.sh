#!/bin/bash
cmake -H. -Bbuild && cmake --build ./build -- -j40 && cd build/test/Release-Linux && ./Vic3ToHoI4Tests && cd ..
