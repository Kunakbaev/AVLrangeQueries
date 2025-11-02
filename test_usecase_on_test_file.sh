#!/bin/bash

cmake --build build --target usecase
./build/source/usecase/usecase < test_input.in > test_output.out
cat test_output.out
