#!/bin/bash

g++ src/main.cpp \
    src/src/*.cpp \
    -Isrc/include \
    -Isrc/api \
    -std=c++11 \
    -o main