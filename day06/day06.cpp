//
// Created by Hadrian Reppas on 12/6/24.
//

#include <string>
#include <vector>
#include <set>
#include <optional>
#include <ranges>

#include "../util/util.h"

static long deltas[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

static std::pair<long, long> findStart(const std::vector<std::string> &grid) {
    for (long i = 0; i < grid.size(); i++)
        for (long j = 0; j < grid[i].size(); j++)
            if (grid[i][j] == '^')
                return std::make_pair(i, j);
    return std::make_pair(0, 0);
}

static std::optional<std::set<std::tuple<long, long, long> > > doWalk(const std::vector<std::string> &grid, long row,
                                                                      long col) {
    long direction = 0;
    std::set<std::tuple<long, long, long> > seen;

    while (true) {
        if (seen.contains({row, col, direction}))
            return std::nullopt;
        seen.emplace(row, col, direction);

        const long newRow = row + deltas[direction][0];
        const long newCol = col + deltas[direction][1];

        if (newRow < 0 || newRow >= grid.size() || newCol < 0 || newCol >= grid[0].size())
            return seen;

        if (grid[newRow][newCol] == '#') {
            direction = (direction + 1) % 4;
        } else {
            row = newRow;
            col = newCol;
        }
    }
}

static std::set<std::pair<long, long> > normalize(std::set<std::tuple<long, long, long> > seen) {
    std::set<std::pair<long, long> > out;
    for (const auto &[row, col, _]: seen)
        out.emplace(row, col);
    return out;
}

namespace day06 {
    long part1(const std::string &input) {
        std::vector<std::string> grid = split(input);
        const auto [row, col] = findStart(grid);
        return static_cast<long>(normalize(*doWalk(grid, row, col)).size());
    }

    long part2(const std::string &input) {
        std::vector<std::string> grid = split(input);
        const auto [row, col] = findStart(grid);

        long count = 0;
        const auto pairs = normalize(*doWalk(grid, row, col));
        for (const auto &[objectRow, objectCol]: pairs) {
            grid.at(objectRow).at(objectCol) = '#';
            count += !doWalk(grid, row, col).has_value();
            grid[objectRow][objectCol] = '.';
        }
        return count;
    }
}
