# 18.cpp-threads-with-shared-memory-and-callbacks

Demo about threads playing with shared memory and callbacks.

## How to compile

```
g++ -std=c++20 main.cpp Person.cpp -o main 
```

## Console program output

```
$ ./main
Created thread: 140175729231424
Created thread: 140175720777280
Created thread: 140175712323136
[2024-03-24 13:45:09.713] Name: Ale, PPM: 121
[2024-03-24 13:45:09.963] Name: Milei, PPM: 158
[2024-03-24 13:45:09.964] Name: Ale, PPM: 110
[2024-03-24 13:45:10.214] Name: Ale, PPM: 62
[2024-03-24 13:45:10.463] Name: Javi, PPM: 160
[2024-03-24 13:45:10.464] Name: Milei, PPM: 174
[2024-03-24 13:45:10.465] Name: Ale, PPM: 60
[2024-03-24 13:45:10.716] Name: Ale, PPM: 88
[2024-03-24 13:45:10.964] Name: Milei, PPM: 140
[2024-03-24 13:45:10.966] Name: Ale, PPM: 94
[2024-03-24 13:45:11.217] Name: Ale, PPM: 156
[2024-03-24 13:45:11.464] Name: Javi, PPM: 133
[2024-03-24 13:45:11.465] Name: Milei, PPM: 109
[2024-03-24 13:45:11.467] Name: Ale, PPM: 63
[2024-03-24 13:45:11.718] Name: Ale, PPM: 122
[2024-03-24 13:45:11.965] Name: Milei, PPM: 141
[2024-03-24 13:45:11.968] Name: Ale, PPM: 157
[2024-03-24 13:45:12.219] Name: Ale, PPM: 125
[2024-03-24 13:45:12.464] Name: Javi, PPM: 139
[2024-03-24 13:45:12.465] Name: Milei, PPM: 115
[2024-03-24 13:45:12.469] Name: Ale, PPM: 92
[2024-03-24 13:45:12.720] Name: Ale, PPM: 123
[2024-03-24 13:45:12.966] Name: Milei, PPM: 109
[2024-03-24 13:45:12.970] Name: Ale, PPM: 82
[2024-03-24 13:45:13.221] Name: Ale, PPM: 174
[2024-03-24 13:45:13.464] Name: Javi, PPM: 110
[2024-03-24 13:45:13.466] Name: Milei, PPM: 148
[2024-03-24 13:45:13.471] Name: Ale, PPM: 147
[2024-03-24 13:45:13.722] Name: Ale, PPM: 96
[2024-03-24 13:45:13.967] Name: Milei, PPM: 136
[2024-03-24 13:45:13.972] Name: Ale, PPM: 169
[2024-03-24 13:45:14.222] Name: Ale, PPM: 67
[2024-03-24 13:45:14.464] Name: Javi, PPM: 114
[2024-03-24 13:45:14.467] Name: Milei, PPM: 129
[2024-03-24 13:45:14.473] Name: Ale, PPM: 100
[2024-03-24 13:45:14.724] Name: Ale, PPM: 124
[2024-03-24 13:45:14.967] Name: Milei, PPM: 122
[2024-03-24 13:45:14.974] Name: Ale, PPM: 101
[2024-03-24 13:45:15.224] Name: Ale, PPM: 62
[2024-03-24 13:45:15.465] Name: Javi, PPM: 81
[2024-03-24 13:45:15.468] Name: Milei, PPM: 166
[2024-03-24 13:45:15.474] Name: Ale, PPM: 158
[2024-03-24 13:45:15.725] Name: Ale, PPM: 64
[2024-03-24 13:45:15.969] Name: Milei, PPM: 94
[2024-03-24 13:45:15.976] Name: Ale, PPM: 71
[2024-03-24 13:45:16.226] Name: Ale, PPM: 158
[2024-03-24 13:45:16.465] Name: Javi, PPM: 86
[2024-03-24 13:45:16.469] Name: Milei, PPM: 168
[2024-03-24 13:45:16.477] Name: Ale, PPM: 133
[2024-03-24 13:45:16.727] Name: Ale, PPM: 165
[2024-03-24 13:45:16.970] Name: Milei, PPM: 134
[2024-03-24 13:45:16.978] Name: Ale, PPM: 165
[2024-03-24 13:45:17.228] Name: Ale, PPM: 139
Deatached thread: 140175729231424
Deatached thread successfully.
Deatached thread: 140175720777280
Deatached thread successfully.
Deatached thread: 140175712323136
Deatached thread successfully.
$
```