#!/bin/bash

g++ src/main.cpp \
    src/src/*.cpp \
    -Isrc/include \
    -Isrc/api \
    -o main