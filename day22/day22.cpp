//
// Created by Hadrian Reppas on 12/29/24.
//

#include <string>
#include <vector>
#include <map>
#include <set>
#include <ranges>
#include <algorithm>

#include "../util/util.h"

static std::vector<long> parseInput(const std::string &input) {
    std::vector<long> out;
    for (const auto &line: split(input))
        out.push_back(std::stoi(line));
    return out;
}

static long prune(const long x) { return x % 16777216; }
static long mix(const long x, const long y) { return x ^ y; }

static long step(long x) {
    x = prune(mix(64 * x, x));
    x = prune(mix(x / 32, x));
    x = prune(mix(2048 * x, x));
    return x;
}

static std::tuple<long, std::vector<long>, std::vector<long> > simulate(long x) {
    long prev = x % 10;
    std::vector<long> prices, diffs;
    for (long i = 0; i < 2000; i++) {
        x = step(x);
        const auto price = x % 10;
        prices.push_back(price);
        diffs.push_back(price - prev);
        prev = price;
    }
    return std::make_tuple(x, prices, diffs);
}

static void evaluate(const std::vector<long> &prices, const std::vector<long> &diffs,
                     std::map<std::tuple<long, long, long, long>, long> &bananas) {
    std::set<std::tuple<long, long, long, long> > seen;
    for (long i = 0; i < prices.size() - 3; i++) {
        const auto run = std::make_tuple(diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]);
        if (seen.contains(run))
            continue;
        seen.insert(run);
        bananas[run] += prices[i + 3];
    }
}

namespace day22 {
    long part1(const std::string &input) {
        const auto xs = parseInput(input);
        long total = 0;
        for (const auto x: xs)
            total += std::get<0>(simulate(x));
        return total;
    }

    long part2(const std::string &input) {
        const auto xs = parseInput(input);
        std::map<std::tuple<long, long, long, long>, long> bananas;
        for (const auto x: xs) {
            const auto [_, prices, diffs] = simulate(x);
            evaluate(prices, diffs, bananas);
        }
        return std::ranges::max(bananas | std::ranges::views::values);
    }
}
