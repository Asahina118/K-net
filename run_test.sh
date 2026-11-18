#!/usr/bin/env bash

set -e
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --verbose
ctest --test-dir build --verbose
