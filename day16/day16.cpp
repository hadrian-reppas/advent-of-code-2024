//
// Created by Hadrian Reppas on 12/16/24.
//

#include <string>
#include <vector>
#include <array>
#include <queue>
#include <functional>
#include <unordered_set>
#include <set>

#include "../util/util.h"

static std::array<std::array<long, 2>, 4> deltas{
    std::array<long, 2>{0, 1},
    std::array<long, 2>{1, 0},
    std::array<long, 2>{0, -1},
    std::array<long, 2>{-1, 0},
};

static std::vector<std::vector<bool> > parseInput(const std::string &input) {
    std::vector<std::vector<bool> > grid;
    for (const auto &line: split(input)) {
        std::vector<bool> row;
        for (const auto &c: line)
            row.push_back(c == '#');
        grid.push_back(row);
    }
    return grid;
}

static std::pair<long, long> turns(const long dir) {
    if (dir == 0)
        return std::make_pair(1, 3);
    if (dir == 3)
        return std::make_pair(2, 0);
    return std::make_pair(dir - 1, dir + 1);
}

struct Node {
    long row, col, dir;

    bool operator==(const Node &other) const {
        return row == other.row && col == other.col && dir == other.dir;
    }

    Node step() const {
        const auto delta = deltas[dir];
        return Node{row + delta[0], col + delta[1], dir};
    }

    std::pair<Node, Node> rotate() const {
        const auto [dir1, dir2] = turns(dir);
        return std::make_pair(
            Node{row, col, dir1},
            Node{row, col, dir2}
        );
    }
};

struct NodeHash {
    std::size_t operator()(const Node &n) const {
        return std::hash<long>()(n.row) ^ std::hash<long>()(n.col + 319) ^ std::hash<long>()(n.dir + 1193);
    }
};

struct NodeInfo {
    long score;
    std::unordered_set<Node, NodeHash> preds;
};

namespace day16 {
    long part1(const std::string &input) {
        const auto grid = parseInput(input);
        auto cmp = [](const auto &left, const auto &right) { return std::get<0>(left) > std::get<0>(right); };
        std::priority_queue<std::pair<long, Node>, std::vector<std::pair<long, Node> >, decltype(cmp)> pq{cmp};
        std::unordered_set<Node, NodeHash> seen;

        const Node start{static_cast<long>(grid.size()) - 2, 1, 0};
        pq.emplace(0, start);

        while (true) {
            const auto [score, node] = pq.top();
            pq.pop();

            if (node.row == 1 && node.col == grid[0].size() - 2)
                return score;

            if (seen.contains(node))
                continue;
            seen.insert(node);

            const auto stepped = node.step();
            if (!grid[stepped.row][stepped.col])
                pq.emplace(score + 1, stepped);

            const auto [rotated1, rotated2] = node.rotate();
            pq.emplace(score + 1000, rotated1);
            pq.emplace(score + 1000, rotated2);
        }
    }

    long part2(const std::string &input) {
        const auto grid = parseInput(input);
        auto cmp = [](const auto &left, const auto &right) { return std::get<0>(left) > std::get<0>(right); };
        std::priority_queue<
            std::tuple<long, Node, Node>,
            std::vector<std::tuple<long, Node, Node> >,
            decltype(cmp)> pq{cmp};
        std::unordered_map<Node, NodeInfo, NodeHash> seen;
        constexpr Node dummy{0, 0, 0};
        long minScore = -1;
        std::unordered_set<Node, NodeHash> stackSet;

        const Node start{static_cast<long>(grid.size()) - 2, 1, 0};
        pq.emplace(0, start, dummy);

        while (!pq.empty()) {
            const auto [score, node, pred] = pq.top();
            pq.pop();

            if (node.row == 1 && node.col == grid[0].size() - 2 && (minScore == -1 || score == minScore)) {
                minScore = score;
                stackSet.insert(node);
            }

            if (seen.contains(node) && score > seen[node].score)
                continue;
            if (pred != dummy) {
                if (seen.contains(node))
                    seen[node].preds.insert(pred);
                else
                    seen[node] = {score, {pred}};
            }

            const auto stepped = node.step();
            if (!grid[stepped.row][stepped.col])
                pq.emplace(score + 1, stepped, node);

            const auto [rotated1, rotated2] = node.rotate();
            pq.emplace(score + 1000, rotated1, node);
            pq.emplace(score + 1000, rotated2, node);
        }

        std::vector<Node> stack(stackSet.begin(), stackSet.end());
        std::unordered_set<Node, NodeHash> onPath;
        while (!stack.empty()) {
            const auto node = stack.back();
            stack.pop_back();
            if (onPath.contains(node))
                continue;
            onPath.insert(node);

            for (const auto &pred: seen[node].preds)
                stack.push_back(pred);
        }

        std::set<std::pair<long, long> > coords;
        for (const auto &node: onPath)
            coords.insert({node.row, node.col});
        return static_cast<long>(coords.size());
    }
}
