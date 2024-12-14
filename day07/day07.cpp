//
// Created by Hadrian Reppas on 12/14/24.
//

#include <string>
#include <ranges>

#include "../util/util.h"

static std::vector<std::pair<long, std::vector<long> > > parseInput(const std::string &input) {
    std::vector<std::pair<long, std::vector<long> > > result;
    for (const auto &line: split(input)) {
        const auto i = line.find(':');
        auto left = std::stol(line.substr(0, i));
        std::vector<long> right = split(line.substr(i + 2), ' ') | std::views::transform([](const std::string &s) {
            return std::stol(s);
        }) | std::ranges::to<std::vector>();
        result.emplace_back(left, std::move(right));
    }
    return result;
}

static long roundUp(const long b) {
    if (b < 10) return 10;
    if (b < 100) return 100;
    if (b < 1000) return 1000;
    if (b < 10000) return 10000;
    if (b < 100000) return 100000;
    if (b < 1000000) return 1000000;
    if (b < 10000000) return 10000000;
    if (b < 100000000) return 100000000;
    if (b < 1000000000) return 1000000000;
    if (b < 10000000000) return 10000000000;
    if (b < 100000000000) return 100000000000;
    if (b < 1000000000000) return 1000000000000;
    if (b < 10000000000000) return 10000000000000;
    if (b < 100000000000000) return 100000000000000;
    if (b < 1000000000000000) return 1000000000000000;
    if (b < 10000000000000000) return 10000000000000000;
    if (b < 100000000000000000) return 100000000000000000;
    return 1000000000000000000;
}

static bool isSuffix(const long haystack, const long needle) {
    if (haystack < needle)
        return false;
    return (haystack - needle) % roundUp(needle) == 0;
}

static bool isValid(const long target, const std::span<long> values, const bool concat) {
    if (values.size() == 1)
        return target == values[0];
    if (target <= 0)
        return false;
    if (target % values.back() == 0 && isValid(target / values.back(), values.first(values.size() - 1), concat))
        return true;
    if (concat && isSuffix(target, values.back()) && isValid(target / roundUp(values.back()),
                                                             values.first(values.size() - 1), concat))
        return true;
    return isValid(target - values.back(), values.first(values.size() - 1), concat);
}

static long totalCalibration(const std::string &input, bool concat) {
    auto lines = parseInput(input);
    long count = 0;
    for (auto &[target, values]: lines) {
        if (isValid(target, values, concat))
            count += target;
    }
    return count;
}

namespace day07 {
    long part1(const std::string &input) {
        return totalCalibration(input, false);
    }

    long part2(const std::string &input) {
        return totalCalibration(input, true);
    }
}
