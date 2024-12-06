//
// Created by Hadrian Reppas on 12/6/24.
//

#include <string>
#include <vector>
#include <sstream>
#include <print>
#include <ranges>
#include <bits/ranges_algo.h>

static std::vector<std::string> parseInput(const std::string &input) {
    std::vector<std::string> grid;
    std::stringstream stream(input);
    std::string line;
    while (std::getline(stream, line))
        grid.push_back(line);
    return grid;
}

static int part1Deltas[8][4][2] = {
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    {{0, 0}, {1, 1}, {2, 2}, {3, 3}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    {{0, 0}, {-1, 1}, {-2, 2}, {-3, 3}},
    {{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}},
    {{0, 0}, {-1, -1}, {-2, -2}, {-3, -3}},
    {{0, 0}, {0, -1}, {0, -2}, {0, -3}},
    {{0, 0}, {1, -1}, {2, -2}, {3, -3}}
};

static int part2Deltas[4][5][2] = {
    {{0, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}},
    {{0, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}},
    {{0, 0}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}},
    {{0, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}},
};

template<size_t n, size_t m>
static long countAt(const std::vector<std::string> &grid, long i, long j, const int (&deltas)[n][m][2],
                    const std::string_view s) {
    long count = 0;
    for (const auto &delta: deltas) {
        if (auto [di, dj] = delta[3]; i + di < 0 || i + di >= grid.size() || j + dj < 0 || j + dj >= grid[i].size())
            continue;

        count += std::ranges::all_of(delta | std::ranges::views::enumerate, [&grid, i, j, s](const auto &pair) {
            auto [k, d] = pair;
            auto [di, dj] = d;
            return (
                i + di >= 0
                && i + di < grid.size()
                && j + dj >= 0
                && j + dj < grid[i].size()
                && s[k] == grid[i + di][j + dj]);
        });
    }
    return count;
}

namespace day04 {
    void part1(const std::string &input) {
        const auto grid = parseInput(input);
        long count = 0;

        for (long i = 0; i < grid.size(); i++)
            for (long j = 0; j < grid[i].size(); j++)
                count += countAt<8, 4>(grid, i, j, part1Deltas, "XMAS");

        std::println("{}", count);
    }

    void part2(const std::string &input) {
        const auto grid = parseInput(input);
        long count = 0;

        for (long i = 0; i < grid.size(); i++)
            for (long j = 0; j < grid[i].size(); j++)
                count += countAt<4, 5>(grid, i, j, part2Deltas, "AMSMS");

        std::println("{}", count);
    }
}
