# cpp-mediator

Created example based on mediator pattern and API consumption in the form of API.

## How to build

```
. ./build.sh
```

## How to run

```
./mediator
```

## How to generate valgrind report

- Note: You need to have installed `valgrind` executable.

```
valgrind --leak-check=yes --log-file=valgrind-out.txt ./mediator
```