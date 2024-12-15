//
// Created by Hadrian Reppas on 12/15/24.
//

#include <string>
#include <set>
#include <vector>

#include "../util/util.h"

static long deltas[4][2] = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0},
};

static std::vector<std::vector<std::pair<long, long> > > parseRegions(const std::vector<std::string> &grid) {
    std::set<std::pair<long, long> > todo;
    for (long i = 0; i < grid.size(); i++) {
        for (long j = 0; j < grid[i].size(); j++) {
            todo.emplace(i, j);
        }
    }

    std::vector<std::vector<std::pair<long, long> > > regions;
    while (!todo.empty()) {
        const auto [firstI, firstJ] = *todo.begin();
        todo.erase(todo.begin());
        std::vector<std::pair<long, long> > stack{{firstI, firstJ}};
        std::vector<std::pair<long, long> > region;
        while (!stack.empty()) {
            const auto [i, j] = stack.back();
            stack.pop_back();
            region.emplace_back(i, j);
            for (const auto delta: deltas) {
                const auto newI = i + delta[0], newJ = j + delta[1];
                if (todo.contains({newI, newJ}) && grid[i][j] == grid[newI][newJ]) {
                    todo.erase({newI, newJ});
                    stack.emplace_back(newI, newJ);
                }
            }
        }
        regions.push_back(std::move(region));
    }
    return regions;
}

using Border = std::tuple<long, long, long, long>;

static std::vector<Border> getBorders(const long row, const long col,
                                      const std::vector<std::string> &grid) {
    std::vector<Border> borders;
    for (const auto delta: deltas) {
        const long newRow = row + delta[0], newCol = col + delta[1];
        if (newRow < 0 || newRow >= grid.size() || newCol < 0 || newCol >= grid[0].size() ||
            grid[newRow][newCol] != grid[row][col])
            borders.emplace_back(row, col, newRow, newCol);
    }
    return borders;
}

static long getPerimeter(const std::vector<std::pair<long, long> > &region,
                         const std::vector<std::string> &grid) {
    long perimeter = 0;
    for (const auto [row, col]: region)
        perimeter += static_cast<long>(getBorders(row, col, grid).size());
    return perimeter;
}

static std::pair<Border, Border> getNeighbors(Border border) {
    const auto [row1, col1, row2, col2] = border;
    if (row1 == row2) {
        return std::make_pair(
            std::make_tuple(row1 - 1, col1, row2 - 1, col2),
            std::make_tuple(row1 + 1, col1, row2 + 1, col2)
        );
    } else {
        return std::make_pair(
            std::make_tuple(row1, col1 - 1, row2, col2 - 1),
            std::make_tuple(row1, col1 + 1, row2, col2 + 1)
        );
    }
}

static long countEdges(const std::vector<std::pair<long, long> > &region,
                       const std::vector<std::string> &grid) {
    std::set<Border> todo;
    for (const auto [row, col]: region) {
        auto newBorders = getBorders(row, col, grid);
        todo.insert(newBorders.begin(), newBorders.end());
    }

    long edges = 0;
    while (!todo.empty()) {
        edges += 1;
        auto border = *todo.begin();
        todo.erase(todo.begin());
        std::vector<Border> stack{border};
        while (!stack.empty()) {
            border = stack.back();
            stack.pop_back();
            const auto [neighbor1, neighbor2] = getNeighbors(border);
            if (todo.contains(neighbor1)) {
                todo.erase(neighbor1);
                stack.push_back(neighbor1);
            }
            if (todo.contains(neighbor2)) {
                todo.erase(neighbor2);
                stack.push_back(neighbor2);
            }
        }
    }
    return edges;
}

namespace day12 {
    long part1(const std::string &input) {
        const auto grid = split(input);
        const auto regions = parseRegions(grid);

        long price = 0;
        for (const auto &region: regions) {
            const auto area = static_cast<long>(region.size());
            const auto perimeter = getPerimeter(region, grid);
            price += area * perimeter;
        }
        return price;
    }

    long part2(const std::string &input) {
        const auto grid = split(input);
        const auto regions = parseRegions(grid);

        long price = 0;
        for (const auto &region: regions) {
            const auto area = static_cast<long>(region.size());
            const auto edges = countEdges(region, grid);
            price += area * edges;
        }
        return price;
    }
}
