//
// Created by Hadrian Reppas on 12/20/24.
//

#include <string>
#include <vector>
#include <array>

#include "../util/util.h"

static std::tuple<std::vector<std::vector<long> >, std::pair<long, long>, std::pair<long, long> > parseInput(
    const std::string &input) {
    const auto lines = split(input);
    std::pair<long, long> start, end;

    std::vector<std::vector<long> > grid;
    for (long i = 0; i < lines.size(); ++i) {
        std::vector<long> row;
        for (long j = 0; j < lines[0].size(); ++j) {
            if (lines[i][j] == '#') {
                row.push_back(-1);
                continue;
            }

            row.push_back(0);
            if (lines[i][j] == 'S')
                start = std::make_pair(i, j);
            else if (lines[i][j] == 'E')
                end = std::make_pair(i, j);
        }
        grid.push_back(std::move(row));
    }

    return std::make_tuple(grid, start, end);
}

static std::array<std::array<long, 2>, 4> deltas{
    std::array<long, 2>{0, 1},
    {1, 0},
    {0, -1},
    {-1, 0},
};

static std::vector<std::pair<long, long> > findPath(std::vector<std::vector<long> > &grid,
                                                    const std::pair<long, long> start,
                                                    const std::pair<long, long> end) {
    std::pair<long, long> current = start;
    std::vector<std::pair<long, long> > path{start};
    grid[start.first][start.second] = 1;

    while (current != end) {
        const auto [i, j] = current;
        for (const auto &delta: deltas) {
            const long ip = i + delta[0], jp = j + delta[1];
            if (grid[ip][jp] == 0) {
                grid[ip][jp] = grid[i][j] + 1;
                current = std::make_pair(ip, jp);
                path.push_back(current);
                break;
            }
        }
    }

    return path;
}

static long countShortcuts(const std::vector<std::vector<long> > &grid,
                           const std::pair<long, long> start, const long cheat) {
    const long startIndex = grid[start.first][start.second];
    long count = 0;
    for (long di = -cheat; di <= cheat; di++) {
        for (long dj = -cheat; dj <= cheat; dj++) {
            const long i = start.first + di, j = start.second + dj;
            const long d = std::abs(di) + std::abs(dj);
            if (d > cheat || i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size())
                continue;

            if (grid[i][j] - startIndex - d >= 100)
                count += 1;
        }
    }
    return count;
}

static long totalShortcuts(const std::string &input, const long cheat) {
    auto [grid, start, end] = parseInput(input);
    const auto path = findPath(grid, start, end);

    long count = 0;
    for (const auto node: path)
        count += countShortcuts(grid, node, cheat);
    return count;
}

namespace day20 {
    long part1(const std::string &input) {
        return totalShortcuts(input, 2);
    }

    long part2(const std::string &input) {
        return totalShortcuts(input, 20);
    }
}
