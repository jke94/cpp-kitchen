# 18.cpp-threads-with-shared-memory-and-callbacks

Demo about threads playing with shared memory and callbacks.

## A. How to compile

```
g++ -std=c++20 main.cpp Person.cpp -o main 
```

## B. Console program output

```
$ ./main
Created thread: 140319643469376 (Javi)
Created thread: 140319635015232 (Ale)
Created thread: 140319626561088 (Milei)
[2024-03-29 11:16:10.035] Name: Ale, PPM: 121
[2024-03-29 11:16:10.285] Name: Milei, PPM: 158
[2024-03-29 11:16:10.286] Name: Ale, PPM: 110
[2024-03-29 11:16:10.536] Name: Ale, PPM: 62
[2024-03-29 11:16:10.785] Name: Javi, PPM: 160
[2024-03-29 11:16:10.785] Name: Milei, PPM: 174
[2024-03-29 11:16:10.787] Name: Ale, PPM: 60
[2024-03-29 11:16:11.037] Name: Ale, PPM: 88
[2024-03-29 11:16:11.286] Name: Milei, PPM: 140
[2024-03-29 11:16:11.288] Name: Ale, PPM: 94
[2024-03-29 11:16:11.538] Name: Ale, PPM: 156
[2024-03-29 11:16:11.786] Name: Javi, PPM: 133
[2024-03-29 11:16:11.786] Name: Milei, PPM: 109
[2024-03-29 11:16:11.788] Name: Ale, PPM: 63
[2024-03-29 11:16:12.040] Name: Ale, PPM: 122
[2024-03-29 11:16:12.287] Name: Milei, PPM: 141
[2024-03-29 11:16:12.290] Name: Ale, PPM: 157
[2024-03-29 11:16:12.542] Name: Ale, PPM: 125
Created thread: 140319618106944 (Lucas)
Deatached thread: 140319626561088 (Milei)
Deatached thread successfully. (Milei)
[2024-03-29 11:16:12.786] Name: Javi, PPM: 139
[2024-03-29 11:16:12.788] Name: Milei, PPM: 115
[2024-03-29 11:16:12.793] Name: Ale, PPM: 92
[2024-03-29 11:16:13.044] Name: Ale, PPM: 123
[2024-03-29 11:16:13.285] Name: Lucas, PPM: 109
[2024-03-29 11:16:13.294] Name: Ale, PPM: 82
[2024-03-29 11:16:13.545] Name: Ale, PPM: 174
[2024-03-29 11:16:13.786] Name: Lucas, PPM: 110
[2024-03-29 11:16:13.787] Name: Javi, PPM: 148
[2024-03-29 11:16:13.795] Name: Ale, PPM: 147
[2024-03-29 11:16:14.045] Name: Ale, PPM: 96
[2024-03-29 11:16:14.286] Name: Lucas, PPM: 136
[2024-03-29 11:16:14.296] Name: Ale, PPM: 169
[2024-03-29 11:16:14.547] Name: Ale, PPM: 67
Created thread: 140319626561088 (James)
Deatached thread: 140319643469376 (Javi)
Deatached thread successfully. (Javi)
[2024-03-29 11:16:14.787] Name: Lucas, PPM: 114
[2024-03-29 11:16:14.787] Name: Javi, PPM: 129
[2024-03-29 11:16:14.798] Name: Ale, PPM: 100
[2024-03-29 11:16:15.049] Name: Ale, PPM: 124
[2024-03-29 11:16:15.067] Name: James, PPM: 122
[2024-03-29 11:16:15.288] Name: Lucas, PPM: 101
[2024-03-29 11:16:15.299] Name: Ale, PPM: 62
[2024-03-29 11:16:15.347] Name: James, PPM: 81
[2024-03-29 11:16:15.550] Name: Ale, PPM: 166
[2024-03-29 11:16:15.628] Name: James, PPM: 158
[2024-03-29 11:16:15.789] Name: Lucas, PPM: 64
[2024-03-29 11:16:15.801] Name: Ale, PPM: 94
[2024-03-29 11:16:15.908] Name: James, PPM: 71
[2024-03-29 11:16:16.051] Name: Ale, PPM: 158
[2024-03-29 11:16:16.189] Name: James, PPM: 86
[2024-03-29 11:16:16.289] Name: Lucas, PPM: 168
[2024-03-29 11:16:16.302] Name: Ale, PPM: 133
[2024-03-29 11:16:16.470] Name: James, PPM: 165
[2024-03-29 11:16:16.553] Name: Ale, PPM: 134
[2024-03-29 11:16:16.751] Name: James, PPM: 165
[2024-03-29 11:16:16.790] Name: Lucas, PPM: 139
[2024-03-29 11:16:16.803] Name: Ale, PPM: 62
[2024-03-29 11:16:17.032] Name: James, PPM: 67
[2024-03-29 11:16:17.054] Name: Ale, PPM: 132
[2024-03-29 11:16:17.290] Name: Lucas, PPM: 143
[2024-03-29 11:16:17.304] Name: Ale, PPM: 65
[2024-03-29 11:16:17.312] Name: James, PPM: 98
[2024-03-29 11:16:17.555] Name: Ale, PPM: 89
[2024-03-29 11:16:17.593] Name: James, PPM: 141
Created thread: 140319643469376 (Lisa)
[2024-03-29 11:16:17.790] Name: Lucas, PPM: 86
[2024-03-29 11:16:17.806] Name: Ale, PPM: 97
[2024-03-29 11:16:17.873] Name: James, PPM: 74
[2024-03-29 11:16:18.056] Name: Ale, PPM: 66
[2024-03-29 11:16:18.154] Name: James, PPM: 168
[2024-03-29 11:16:18.187] Name: Lisa, PPM: 169
[2024-03-29 11:16:18.291] Name: Lucas, PPM: 128
[2024-03-29 11:16:18.306] Name: Ale, PPM: 119
[2024-03-29 11:16:18.434] Name: James, PPM: 171
[2024-03-29 11:16:18.558] Name: Ale, PPM: 60
[2024-03-29 11:16:18.588] Name: Lisa, PPM: 136
[2024-03-29 11:16:18.715] Name: James, PPM: 179
Deatached thread: 140319635015232 (Ale)
Deatached thread successfully. (Ale)
[2024-03-29 11:16:18.793] Name: Lucas, PPM: 64
[2024-03-29 11:16:18.808] Name: Ale, PPM: 170
[2024-03-29 11:16:18.988] Name: Lisa, PPM: 70
[2024-03-29 11:16:18.996] Name: James, PPM: 72
[2024-03-29 11:16:19.277] Name: James, PPM: 106
[2024-03-29 11:16:19.293] Name: Lucas, PPM: 88
[2024-03-29 11:16:19.390] Name: Lisa, PPM: 146
[2024-03-29 11:16:19.558] Name: James, PPM: 122
[2024-03-29 11:16:19.790] Name: Lisa, PPM: 72
[2024-03-29 11:16:19.794] Name: Lucas, PPM: 130
[2024-03-29 11:16:19.839] Name: James, PPM: 80
[2024-03-29 11:16:20.119] Name: James, PPM: 74
[2024-03-29 11:16:20.191] Name: Lisa, PPM: 168
[2024-03-29 11:16:20.295] Name: Lucas, PPM: 62
[2024-03-29 11:16:20.400] Name: James, PPM: 158
[2024-03-29 11:16:20.591] Name: Lisa, PPM: 173
[2024-03-29 11:16:20.681] Name: James, PPM: 100
[2024-03-29 11:16:20.796] Name: Lucas, PPM: 66
[2024-03-29 11:16:20.961] Name: James, PPM: 134
[2024-03-29 11:16:20.992] Name: Lisa, PPM: 157
[2024-03-29 11:16:21.242] Name: James, PPM: 134
[2024-03-29 11:16:21.296] Name: Lucas, PPM: 179
[2024-03-29 11:16:21.393] Name: Lisa, PPM: 163
[2024-03-29 11:16:21.523] Name: James, PPM: 153
Deatached thread: 140319618106944 (Lucas)
Deatached thread successfully. (Lucas)
Deatached thread: 140319626561088 (James)
Deatached thread successfully. (James)
Deatached thread: 140319643469376 (Lisa)
Deatached thread successfully. (Lisa)

$
```

## C. Valgrind: detecting memory leaks.

Install **valgrid** and run the following command:

```
$valgrind --leak-check=yes --log-file=valgrind-out.txt ./main
```
Valgrind output example:

```
==1602== Memcheck, a memory error detector
==1602== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1602== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==1602== Command: ./main
==1602== Parent PID: 9
==1602== 
==1602== error calling PR_SET_PTRACER, vgdb might block
==1602== Thread 4:
==1602== Invalid read of size 8
==1602==    at 0x10DE98: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd62f8 is 72 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BAB6: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BA33: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Thread 2:
==1602== Invalid read of size 8
==1602==    at 0x10DD62: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd4d20 is 32 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BB39: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Invalid read of size 8
==1602==    at 0x10DD66: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd4d28 is 40 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BB39: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Invalid read of size 8
==1602==    at 0x10DD74: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd4d50 is 80 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BB39: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Invalid write of size 4
==1602==    at 0x10DD97: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd4d40 is 64 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BB39: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Invalid read of size 2
==1602==    at 0x48529E0: memmove (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10DEF2: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd4d30 is 48 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BB39: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== Thread 3:
==1602== Invalid read of size 1
==1602==    at 0x4852A10: memmove (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10DEF2: Person::do_live() (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x493E252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x4B42AC2: start_thread (pthread_create.c:442)
==1602==    by 0x4BD3A03: clone (clone.S:100)
==1602==  Address 0x4dd6022 is 50 bytes inside a block of size 88 free'd
==1602==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10CCCF: Monitor::remove_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BBCA: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==  Block was alloc'd at
==1602==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x10D39C: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9EF: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== 
==1602== HEAP SUMMARY:
==1602==     in use at exit: 960 bytes in 6 blocks
==1602==   total heap usage: 325 allocs, 319 frees, 139,913 bytes allocated
==1602== 
==1602== Thread 1:
==1602== 288 bytes in 1 blocks are possibly lost in loss record 4 of 6
==1602==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x40147D9: calloc (rtld-malloc.h:44)
==1602==    by 0x40147D9: allocate_dtv (dl-tls.c:375)
==1602==    by 0x40147D9: _dl_allocate_tls (dl-tls.c:634)
==1602==    by 0x4B437B4: allocate_stack (allocatestack.c:430)
==1602==    by 0x4B437B4: pthread_create@@GLIBC_2.34 (pthread_create.c:647)
==1602==    by 0x493E328: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x10E36E: Person::Person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, void (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int)) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10D431: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10B9A9: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== 288 bytes in 1 blocks are possibly lost in loss record 5 of 6
==1602==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x40147D9: calloc (rtld-malloc.h:44)
==1602==    by 0x40147D9: allocate_dtv (dl-tls.c:375)
==1602==    by 0x40147D9: _dl_allocate_tls (dl-tls.c:634)
==1602==    by 0x4B437B4: allocate_stack (allocatestack.c:430)
==1602==    by 0x4B437B4: pthread_create@@GLIBC_2.34 (pthread_create.c:647)
==1602==    by 0x493E328: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x10E36E: Person::Person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, void (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int)) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10D431: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BA33: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== 288 bytes in 1 blocks are possibly lost in loss record 6 of 6
==1602==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1602==    by 0x40147D9: calloc (rtld-malloc.h:44)
==1602==    by 0x40147D9: allocate_dtv (dl-tls.c:375)
==1602==    by 0x40147D9: _dl_allocate_tls (dl-tls.c:634)
==1602==    by 0x4B437B4: allocate_stack (allocatestack.c:430)
==1602==    by 0x4B437B4: pthread_create@@GLIBC_2.34 (pthread_create.c:647)
==1602==    by 0x493E328: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==1602==    by 0x10E36E: Person::Person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, void (*)(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int)) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10D431: Monitor::add_person(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int) (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602==    by 0x10BA7E: main (in /mnt/j/Repositories/cpp-kitchen/18.cpp-threads-with-shared-memory-and-callbacks/build/main)
==1602== 
==1602== LEAK SUMMARY:
==1602==    definitely lost: 0 bytes in 0 blocks
==1602==    indirectly lost: 0 bytes in 0 blocks
==1602==      possibly lost: 864 bytes in 3 blocks
==1602==    still reachable: 96 bytes in 3 blocks
==1602==         suppressed: 0 bytes in 0 blocks
==1602== Reachable blocks (those to which a pointer was found) are not shown.
==1602== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==1602== 
==1602== For lists of detected and suppressed errors, rerun with: -s
==1602== ERROR SUMMARY: 18 errors from 10 contexts (suppressed: 0 from 0)

```