//
// Created by Hadrian Reppas on 12/1/24.
//

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <ranges>
#include <unordered_map>

#include "../util/util.h"

static std::pair<std::vector<long>, std::vector<long> > parseInput(const std::string &input) {
    std::vector<long> left, right;
    for (const auto &line: split(input)) {
        left.push_back(std::stol(line.substr(0, 5)));
        right.push_back(std::stol(line.substr(8)));
    }
    return std::make_pair(left, right);
}

namespace day01 {
    long part1(const std::string &input) {
        auto [left, right] = parseInput(input);

        std::ranges::sort(left);
        std::ranges::sort(right);

        auto diff = [](const auto &pair) {
            const auto [l, r] = pair;
            return std::abs(l - r);
        };
        auto deltas = std::views::zip(left, right)
                      | std::views::transform(diff);
        return std::accumulate(std::begin(deltas), std::end(deltas), 0L);
    }

    long part2(const std::string &input) {
        auto [left, right] = parseInput(input);
        std::unordered_map<long, long> rightCounts;
        for (const auto r: right)
            rightCounts[r]++;

        auto scores = left | std::views::transform([&rightCounts](const auto l) {
            return l * rightCounts[l];
        });
        return std::accumulate(std::begin(scores), std::end(scores), 0L);
    }
}
