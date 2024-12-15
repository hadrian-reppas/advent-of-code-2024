//
// Created by Hadrian Reppas on 12/15/24.
//

#include <ranges>
#include <string>
#include <vector>
#include <map>

#include "../util/util.h"

static std::vector<long> parseInput(const std::string &input) {
    auto tokens = split(input, ' ');
    return tokens | std::ranges::views::transform([](const auto &token) {
        return std::stol(token);
    }) | std::ranges::to<std::vector<long> >();
}

static long countDigits(const long x) {
    if (x < 10) return 1;
    if (x < 100) return 2;
    if (x < 1000) return 3;
    if (x < 10000) return 4;
    if (x < 100000) return 5;
    if (x < 1000000) return 6;
    if (x < 10000000) return 7;
    if (x < 100000000) return 8;
    if (x < 1000000000) return 9;
    if (x < 10000000000) return 10;
    if (x < 100000000000) return 11;
    if (x < 1000000000000) return 12;
    if (x < 10000000000000) return 13;
    if (x < 100000000000000) return 14;
    if (x < 1000000000000000) return 15;
    if (x < 10000000000000000) return 16;
    if (x < 100000000000000000) return 17;
    return 18;
}

static long denom(const long x) {
    if (x < 100) return 10;
    if (x < 10000) return 100;
    if (x < 1000000) return 1000;
    if (x < 100000000) return 10000;
    if (x < 10000000000) return 100000;
    if (x < 1000000000000) return 1000000;
    if (x < 100000000000000) return 10000000;
    if (x < 10000000000000000) return 100000000;
    return 1000000000;
}

static long countChildren(const long stone, const long steps, std::map<std::pair<long, long>, long> &cache) {
    if (steps == 0)
        return 1;
    if (cache.contains({stone, steps}))
        return cache[{stone, steps}];

    if (stone == 0) {
        const auto children = countChildren(1, steps - 1, cache);
        return cache[{stone, steps}] = children;
    } else if (countDigits(stone) % 2 == 0) {
        const auto d = denom(stone);
        const auto leftChildren = countChildren(stone / d, steps - 1, cache);
        const auto rightChildren = countChildren(stone % d, steps - 1, cache);
        return cache[{stone, steps}] = leftChildren + rightChildren;
    } else {
        const auto children = countChildren(2024 * stone, steps - 1, cache);
        return cache[{stone, steps}] = children;
    }
}

static long countAllChildren(const std::vector<long> &stones, long steps) {
    std::map<std::pair<long, long>, long> cache;
    long total = 0;
    for (const auto stone: stones)
        total += countChildren(stone, steps, cache);
    return total;
}

namespace day11 {
    long part1(const std::string &input) {
        const auto stones = parseInput(input);
        return countAllChildren(stones, 25);
    }

    long part2(const std::string &input) {
        const auto stones = parseInput(input);
        return countAllChildren(stones, 75);
    }
}
