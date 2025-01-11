#include <iostream>

#ifndef ALPHA_VALUE
#define ALPHA_VALUE 80
#endif

int main()
{
    std::cout << "The value: " << ALPHA_VALUE << std::endl;

    return 0;
}

/*

EXAMPLE A:

    A.1. COMPILATION CASE A:

        $ g++ main.cpp -o main

    A.2. CONSOLE OUTPUT:

        The value: 80

EXAMPLE B:

    B.1. COMPILATION CASE B:

        $ g++ -DALPHA_VALUE=1994 main.cpp -o main

    B.2. CONSOLE OUTPUT:

        The value: 1994
*/