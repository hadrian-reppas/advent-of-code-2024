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
    // RUN(day06);
    RUN(day07);
    RUN(day08);
    RUN(day09);

    return 0;
}
