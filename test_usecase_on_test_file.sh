#!/bin/bash

cmake --build build --target avl_usecase
./build/source/usecase/avl_usecase < test_input.in > test_output.out
cat test_output.out
