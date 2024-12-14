//
// Created by Hadrian Reppas on 12/14/24.
//

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <ranges>

#include "../util/util.h"

static std::unordered_map<char, std::vector<std::pair<long, long> > > parseAntennaGroups(
    const std::vector<std::string> &grid) {
    std::unordered_map<char, std::vector<std::pair<long, long> > > antennaGroups;
    for (long i = 0; i < grid.size(); i++) {
        for (long j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '.')
                antennaGroups[grid[i][j]].emplace_back(i, j);
        }
    }
    return antennaGroups;
}

static bool inBounds(const long i, const long j, const std::vector<std::string> &grid) {
    return i >= 0 && j >= 0 && i < grid.size() && j < grid[0].size();
}

namespace day08 {
    long part1(const std::string &input) {
        const std::vector<std::string> grid = split(input);
        const auto antennaGroups = parseAntennaGroups(grid);

        std::set<std::pair<long, long> > nodes;
        for (const auto &antennas: std::views::values(antennaGroups)) {
            for (long i = 0; i < antennas.size(); i++) {
                const auto [x1, y1] = antennas[i];
                for (long j = 0; j < i; j++) {
                    const auto [x2, y2] = antennas[j];
                    const auto dx = x2 - x1;
                    const auto dy = y2 - y1;
                    if (inBounds(x1 - dx, y1 - dy, grid))
                        nodes.emplace(x1 - dx, y1 - dy);
                    if (inBounds(x2 + dx, y2 + dy, grid))
                        nodes.emplace(x2 + dx, y2 + dy);
                }
            }
        }
        return static_cast<long>(nodes.size());
    }

    long part2(const std::string &input) {
        const std::vector<std::string> grid = split(input);
        const auto antennaGroups = parseAntennaGroups(grid);

        std::set<std::pair<long, long> > nodes;
        for (const auto &antennas: std::views::values(antennaGroups)) {
            for (long i = 0; i < antennas.size(); i++) {
                const auto [x1, y1] = antennas[i];
                for (long j = 0; j < i; j++) {
                    const auto [x2, y2] = antennas[j];
                    const auto dx = x2 - x1;
                    const auto dy = y2 - y1;

                    auto x = x1;
                    auto y = y1;
                    while (inBounds(x, y, grid)) {
                        nodes.emplace(x, y);
                        x -= dx;
                        y -= dy;
                    }

                    x = x2;
                    y = y2;
                    while (inBounds(x, y, grid)) {
                        nodes.emplace(x, y);
                        x += dx;
                        y += dy;
                    }
                }
            }
        }
        return static_cast<long>(nodes.size());
    }
}
