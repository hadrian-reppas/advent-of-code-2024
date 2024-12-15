cmake_minimum_required(VERSION 3.29)
project(advent_of_code_2024)

set(CMAKE_CXX_STANDARD 23)

add_executable(advent_of_code_2024 main.cpp
        util/util.h
        util/util.cpp
        day01/day01.h
        day01/day01.cpp
        day02/day02.h
        day02/day02.cpp
        day03/day03.h
        day03/day03.cpp
        day04/day04.h
        day04/day04.cpp
        day05/day05.h
        day05/day05.cpp
        day06/day06.h
        day06/day06.cpp
        day07/day07.h
        day07/day07.cpp
        day08/day08.h
        day08/day08.cpp
        day09/day09.h
        day09/day09.cpp
        day10/day10.h
        day10/day10.cpp)
