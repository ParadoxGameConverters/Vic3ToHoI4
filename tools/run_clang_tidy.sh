#!/bin/bash
rm -rf build/
cmake -H. -Bbuild && cmake --build ./build --target run_clang_tidy