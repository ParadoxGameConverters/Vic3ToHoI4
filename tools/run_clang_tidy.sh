#!/bin/bash
cmake -H. -Bbuild && cmake --build ./build --target run_clang_tidy