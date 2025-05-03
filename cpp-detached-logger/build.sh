#!/bin/bash

g++ ./src/main.cpp \
    ./src/src/*.cpp \
    -I./src/api \
    -I./src/include \
    -std=c++14 \
    -o ./cpp-detached-logger