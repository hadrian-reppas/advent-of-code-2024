//
// Created by Hadrian Reppas on 12/31/24.
//

#include <string>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>

#include "../util/util.h"

static std::pair<std::vector<std::array<long, 5> >, std::vector<std::array<long, 5> > > parseInput(
    const std::string &input) {
    const auto lines = split(input);
    std::vector<std::array<long, 5> > locks, keys;
    for (long i = 0; i < lines.size(); i += 8) {
        const auto isLock = lines[i][0] == '#';
        auto scan = isLock ? i + 1 : i + 5;
        const auto dScan = isLock ? 1 : -1;
        std::array<long, 5> result{-1, -1, -1, -1, -1};
        for (long j = 0; j < 6; j++) {
            for (long k = 0; k < 5; k++) {
                if (lines[scan][k] == '.' && result[k] == -1)
                    result[k] = j;
            }
            scan += dScan;
        }
        if (isLock)
            locks.push_back(result);
        else
            keys.push_back(result);
    }
    return std::make_pair(locks, keys);
}

static bool fits(const std::array<long, 5> &lock, const std::array<long, 5> &key) {
    return std::ranges::all_of(
        std::views::zip(lock, key),
        [](const auto &pair) { return std::get<0>(pair) + std::get<1>(pair) <= 5; }
    );
}

namespace day25 {
    long part1(const std::string &input) {
        const auto [locks, keys] = parseInput(input);
        long count = 0;
        for (const auto &lock: locks) {
            for (const auto &key: keys) {
                count += fits(lock, key);
            }
        }
        return count;
    }

    std::string part2(const std::string &input) {
        return "*";
    }
}
