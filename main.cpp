#include <fstream>
#include <string>
#include <print>

#include "day01/day01.h"
#include "day02/day02.h"
#include "day03/day03.h"
#include "day04/day04.h"
#include "day05/day05.h"
#include "day06/day06.h"
#include "day07/day07.h"
#include "day08/day08.h"
#include "day09/day09.h"
#include "day10/day10.h"
#include "day11/day11.h"
#include "day12/day12.h"
#include "day13/day13.h"
#include "day14/day14.h"
#include "day15/day15.h"
#include "day16/day16.h"
#include "day17/day17.h"
#include "day18/day18.h"
#include "day19/day19.h"
#include "day20/day20.h"
#include "day21/day21.h"
#include "day22/day22.h"
#include "day23/day23.h"
#include "day24/day24.h"
#include "day25/day25.h"

#define RUN(DAY)                                                        \
    do {                                                                \
        std::println("================== " #DAY " =================="); \
        std::ifstream file("../inputs/" #DAY ".txt");                   \
        const std::string input((std::istreambuf_iterator<char>(file)), \
                                std::istreambuf_iterator<char>());      \
        std::println("{}", DAY::part1(input));                          \
        std::println("{}", DAY::part2(input));                          \
    } while (0)

int main() {
    RUN(day01);
    RUN(day02);
    RUN(day03);
    RUN(day04);
    RUN(day05);
    RUN(day06);
    RUN(day07);
    RUN(day08);
    RUN(day09);
    RUN(day10);
    RUN(day11);
    RUN(day12);
    RUN(day13);
    RUN(day14);
    RUN(day15);
    RUN(day16);
    RUN(day17);
    RUN(day18);
    RUN(day19);
    RUN(day20);
    RUN(day21);
    RUN(day22);
    RUN(day23);
    RUN(day24);
    RUN(day25);

    return 0;
}
