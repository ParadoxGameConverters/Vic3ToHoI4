#!/bin/bash
rm -rf build/
cmake --preset x64-release-linux
cmake --build --preset build-x64-release-linux --target Vic3ToHoi4 Vic3ToHoi4Tests -- -j40
cmake --build --preset build-x64-release-linux --target run_clang_tidy -- -j40