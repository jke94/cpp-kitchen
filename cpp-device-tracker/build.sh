#!/bin/bash

g++ ./src/main.cpp \
    ./src/src/*.cpp \
    -I./src/api \
    -I./src/include \
    -std=c++11 \
    -o ./device-tracker