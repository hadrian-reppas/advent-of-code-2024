//
// Created by Hadrian Reppas on 12/14/24.
//

#include <string>
#include <set>

#include "../util/util.h"

static long deltas[4][2] = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0},
};

static long getScore(const std::vector<std::string> &grid, const long i, const long j,
                     std::set<std::pair<long, long> > &seen, bool checkSeen) {
    if (grid[i][j] == '9') {
        if (checkSeen) {
            const auto [_, added] = seen.insert({i, j});
            return added;
        }
        return 1;
    }

    long score = 0;
    for (const auto delta: deltas) {
        const long newI = i + delta[0], newJ = j + delta[1];
        if (newI < 0 || newI >= grid.size() || newJ < 0 || newJ >= grid[0].size())
            continue;
        if (grid[newI][newJ] == grid[i][j] + 1)
            score += getScore(grid, newI, newJ, seen, checkSeen);
    }
    return score;
}

static long totalScore(const std::vector<std::string> &grid, const bool checkSeen) {
    long total = 0;
    for (long i = 0; i < grid.size(); ++i) {
        for (long j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == '0') {
                std::set<std::pair<long, long> > seen;
                total += getScore(grid, i, j, seen, checkSeen);
            }
        }
    }
    return total;
}

namespace day10 {
    long part1(const std::string &input) {
        const auto grid = split(input);
        return totalScore(grid, true);
    }

    long part2(const std::string &input) {
        const auto grid = split(input);
        return totalScore(grid, false);
    }
}
