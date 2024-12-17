//
// Created by Hadrian Reppas on 12/16/24.
//

#include <string>
#include <ranges>
#include <unordered_map>
#include <cassert>

#include "../util/util.h"

enum class Cell {
    Empty,
    Wall,
    Object,
    ObjectLeft,
    ObjectRight,
};

static std::pair<std::vector<Cell>, std::optional<long> > parseLine(const std::string &line, const bool part2) {
    std::vector<Cell> cells;
    std::optional<long> robot;
    for (long i = 0; i < line.size(); i++) {
        if (line[i] == '#') {
            cells.push_back(Cell::Wall);
            if (part2)
                cells.push_back(Cell::Wall);
        } else if (line[i] == 'O')
            if (part2) {
                cells.push_back(Cell::ObjectLeft);
                cells.push_back(Cell::ObjectRight);
            } else {
                cells.push_back(Cell::Object);
            }
        else {
            cells.push_back(Cell::Empty);
            if (part2)
                cells.push_back(Cell::Empty);
        }
        if (line[i] == '@') {
            if (part2)
                robot = std::make_optional(2 * i);
            else
                robot = std::make_optional(i);
        }
    }
    return std::make_pair(cells, robot);
}

static std::tuple<std::vector<std::vector<Cell> >, std::pair<long, long>, std::string> parseInput(
    const std::string &input, const bool part2) {
    const auto lines = split(input);
    std::vector<std::vector<Cell> > grid;
    long i, robotRow, robotCol;
    for (i = 0; ; i++) {
        if (lines[i].empty())
            break;
        const auto [line, robot] = parseLine(lines[i], part2);
        grid.push_back(line);
        if (robot.has_value()) {
            robotRow = i;
            robotCol = robot.value();
        }
    }

    i++;
    std::string moves;
    for (; i < lines.size(); i++)
        moves += lines[i];

    return std::make_tuple(grid, std::make_pair(robotRow, robotCol), moves);
}

static std::unordered_map<char, std::pair<long, long> > deltas{
    {'>', {0, 1}},
    {'v', {1, 0}},
    {'<', {0, -1}},
    {'^', {-1, 0}},
};

static std::pair<long, long> stepPart1(std::vector<std::vector<Cell> > &grid, std::pair<long, long> robot,
                                       const char c) {
    const auto [dRow, dCol] = deltas[c];
    auto [row, col] = robot;
    long nextRow = row = row + dRow, nextCol = col = col + dCol;
    while (grid[row][col] == Cell::Object) {
        row += dRow;
        col += dCol;
    }

    if (grid[row][col] == Cell::Wall)
        return robot;
    if (row == nextRow && col == nextCol)
        return std::make_pair(row, col);

    grid[row][col] = Cell::Object;
    grid[nextRow][nextCol] = Cell::Empty;
    return std::make_pair(nextRow, nextCol);
}

static bool canMove(const std::vector<std::vector<Cell> > &grid, const long row, const long col, const long dRow) {
    if (grid[row][col] == Cell::Wall)
        return false;
    else if (grid[row][col] == Cell::Empty)
        return true;
    else if (grid[row][col] == Cell::ObjectLeft)
        return canMove(grid, row + dRow, col, dRow) && canMove(grid, row + dRow, col + 1, dRow);
    else
        return canMove(grid, row + dRow, col - 1, dRow) && canMove(grid, row + dRow, col, dRow);
}

static void doMove(std::vector<std::vector<Cell> > &grid, const long row, const long col, const long dRow) {
    if (grid[row][col] == Cell::Wall)
        assert(false);
    else if (grid[row][col] == Cell::Empty)
        return;

    if (grid[row][col] == Cell::ObjectLeft) {
        doMove(grid, row + dRow, col, dRow);
        doMove(grid, row + dRow, col + 1, dRow);
        grid[row + dRow][col] = Cell::ObjectLeft;
        grid[row + dRow][col + 1] = Cell::ObjectRight;
        grid[row][col] = Cell::Empty;
        grid[row][col + 1] = Cell::Empty;
    } else {
        doMove(grid, row + dRow, col - 1, dRow);
        doMove(grid, row + dRow, col, dRow);
        grid[row + dRow][col - 1] = Cell::ObjectLeft;
        grid[row + dRow][col] = Cell::ObjectRight;
        grid[row][col - 1] = Cell::Empty;
        grid[row][col] = Cell::Empty;
    }
}

static std::pair<long, long> stepPart2(std::vector<std::vector<Cell> > &grid, std::pair<long, long> robot,
                                       const char c) {
    auto [row, col] = robot;
    if (c == '<') {
        const auto nextCol = col = col - 1;
        while (grid[row][col] == Cell::ObjectLeft || grid[row][col] == Cell::ObjectRight)
            col -= 1;

        if (grid[row][col] == Cell::Wall)
            return robot;

        for (long j = col; j <= nextCol; j++)
            grid[row][j] = grid[row][j + 1];

        return std::make_pair(row, nextCol);
    } else if (c == '>') {
        const auto nextCol = col = col + 1;
        while (grid[row][col] == Cell::ObjectLeft || grid[row][col] == Cell::ObjectRight)
            col += 1;

        if (grid[row][col] == Cell::Wall)
            return robot;

        for (long j = col; j >= nextCol; j--)
            grid[row][j] = grid[row][j - 1];

        return std::make_pair(row, nextCol);
    } else if (c == 'v') {
        if (!canMove(grid, row + 1, col, 1))
            return robot;
        doMove(grid, row + 1, col, 1);
        return std::make_pair(row + 1, col);
    } else {
        if (!canMove(grid, row - 1, col, -1))
            return robot;
        doMove(grid, row - 1, col, -1);
        return std::make_pair(row - 1, col);
    }
}

static long totalGps(const std::vector<std::vector<Cell> > &grid) {
    long total = 0;
    for (long i = 0; i < grid.size(); i++) {
        for (long j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == Cell::Object || grid[i][j] == Cell::ObjectLeft)
                total += 100 * i + j;
        }
    }
    return total;
}

namespace day15 {
    long part1(const std::string &input) {
        auto [grid, robot, moves] = parseInput(input, false);

        for (const auto c: moves)
            robot = stepPart1(grid, robot, c);

        return totalGps(grid);
    }

    long part2(const std::string &input) {
        auto [grid, robot, moves] = parseInput(input, true);

        for (const auto c: moves)
            robot = stepPart2(grid, robot, c);

        return totalGps(grid);
    }
}
