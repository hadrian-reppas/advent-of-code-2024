#include <fstream>
#include <string>
#include <print>

#include "day01/day01.h"
#include "day02/day02.h"

#define RUN(DAY)                                                        \
    do {                                                                \
        std::println("================== " #DAY " =================="); \
        std::ifstream file("../inputs/" #DAY ".txt");                   \
        const std::string input((std::istreambuf_iterator<char>(file)), \
                                std::istreambuf_iterator<char>());      \
        DAY::part1(input);                                              \
        DAY::part2(input);                                              \
    } while (0)

int main() {
    RUN(day01);
    RUN(day02);

    return 0;
}
