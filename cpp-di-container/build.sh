#!/bin/bash

g++ ./src/src/*.cpp \
    ./src/main.cpp \
    -I./src/api \
    -I./src/include \
    -o ./cpp-di-container-sample