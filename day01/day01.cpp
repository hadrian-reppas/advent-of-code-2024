//
// Created by Hadrian Reppas on 12/1/24.
//

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
#include <ranges>
#include <unordered_map>
#include <print>

std::pair<std::vector<long>, std::vector<long> > parseInput(const std::string &input) {
    std::stringstream stream(input);
    std::string line;
    std::vector<long> left, right;

    while (std::getline(stream, line)) {
        std::stringstream lineStream(line);
        long l, r;
        lineStream >> l >> r;
        left.push_back(l);
        right.push_back(r);
    }

    return std::make_pair(left, right);
}

namespace day01 {
    void part1(const std::string &input) {
        auto [left, right] = parseInput(input);

        std::ranges::sort(left);
        std::ranges::sort(right);

        auto diff = [](const auto &pair) {
            const auto [l, r] = pair;
            return std::abs(l - r);
        };
        auto deltas = std::views::zip(left, right)
                      | std::views::transform(diff);
        const auto sum = std::accumulate(std::begin(deltas), std::end(deltas), 0L);
        std::println("{}", sum);
    }

    void part2(const std::string &input) {
        auto [left, right] = parseInput(input);
        std::unordered_map<long, long> rightCounts;
        for (const auto r: right)
            rightCounts[r]++;

        auto scores = left | std::views::transform([&rightCounts](const auto l) {
            return l * rightCounts[l];
        });
        auto sum = std::accumulate(std::begin(scores), std::end(scores), 0LL);
        std::println("{}", sum);
    }
}
