cmake_minimum_required(VERSION 3.29)
project(advent_of_code_2024)

set(CMAKE_CXX_STANDARD 23)

include_directories(/opt/homebrew/Cellar/z3/4.13.4/include)
link_directories(/opt/homebrew/Cellar/z3/4.13.4/lib)

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
        day10/day10.cpp
        day11/day11.h
        day11/day11.cpp
        day12/day12.h
        day12/day12.cpp
        day13/day13.h
        day13/day13.cpp
        day14/day14.h
        day14/day14.cpp
        day15/day15.h
        day15/day15.cpp
        day16/day16.h
        day16/day16.cpp
        day17/day17.h
        day17/day17.cpp)

target_link_libraries(advent_of_code_2024 PRIVATE z3)
