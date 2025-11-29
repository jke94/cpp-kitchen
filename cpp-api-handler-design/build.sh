#!/bin/bash

# Generate client example consuming the API library design based on handler pattern.
g++ ./src/src/*.cpp \
    ./src/main.cpp \
    -std=c++17 \
    -I./src/api \
    -I./src/include \
    -o ./main