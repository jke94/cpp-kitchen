#!/bin/bash

valgrind --tool=helgrind \
    --verbose \
    --log-file=cpp-task-engine.valgrind-helgrind-out.txt \
    ./main