#include <iostream>
#include <memory>
#include <vector>

/**
 *  Summary: C++ codeusing unique pointers to avoid memory leaks.
 *  GitHub author: https://github.com/jke94
 *  Date: 2023, October.
*/

class Point3D
{
public:
    Point3D(double x, double y, double z) : x(x), y(y), z(){};

private:
    double x{}, y{}, z{};
};

int main()
{    
    // Example A: Code without unique pointers.

    std::vector<Point3D*>* p_vector = new std::vector<Point3D*>{
        new Point3D(1.3, -2.5, 2.6),
        new Point3D(5.8, 5.3, -5.3)
    };

    for(auto i : *p_vector)
    {
        delete i;
    }

    delete p_vector;

    // Example B: Equivalent code, with unique pointers (no delete actions needed).

    std::unique_ptr<std::vector<std::unique_ptr<Point3D>>> p_unique_prt_vector {
        new std::vector<std::unique_ptr<Point3D>>
    };
    
    p_unique_prt_vector->push_back(static_cast<std::unique_ptr<Point3D>>(new Point3D(1.3, -2.5, 2.6)));
    p_unique_prt_vector->push_back(static_cast<std::unique_ptr<Point3D>>(new Point3D(5.8, 5.3, -5.3)));

    return 0;
}

/*
//////////////////////////
Valgrind console output:
//////////////////////////

$ g++ -std=c++11 main.cpp -o main
$ valgrind ./main
==2096== Memcheck, a memory error detector
==2096== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==2096== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2096== Command: ./main
==2096==
==2096== error calling PR_SET_PTRACER, vgdb might block
==2096== 
==2096== HEAP SUMMARY:
==2096==     in use at exit: 0 bytes in 0 blocks
==2096==   total heap usage: 10 allocs, 10 frees, 72,888 bytes allocated
==2096==
==2096== All heap blocks were freed -- no leaks are possible
==2096==
==2096== For lists of detected and suppressed errors, rerun with: -s
==2096== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
$
*/