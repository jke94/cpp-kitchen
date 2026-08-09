#!/bin/bash

valgrind --tool=drd \
    --verbose \
    --log-file=cpp-task-engine.valgrind-drd-out.txt \
    ./main