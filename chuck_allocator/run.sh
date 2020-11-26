#!/bin/bash

set -e

g++ -std=c++14 -I./ test.cpp -o test
./test

echo All tests passed!