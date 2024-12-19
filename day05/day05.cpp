//
// Created by Hadrian Reppas on 12/6/24.
//

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <ranges>

#include "../util/util.h"

static std::pair<std::vector<std::pair<long, long> >, std::vector<std::vector<long> > > parseInput(
    const std::string &input) {
    std::vector<std::pair<long, long> > rules;

    const auto lines = split(input);
    long i = 0;
    for (; !lines[i].empty(); i++) {
        const auto parts = split(lines[i], '|');
        rules.emplace_back(std::stol(parts[0]), std::stol(parts[1]));
    }

    std::vector<std::vector<long> > updates;
    i += 1;
    for (; i < lines.size(); i++) {
        updates.push_back(
            split(lines[i], ',')
            | std::ranges::views::transform([](const auto &part) { return std::stol(part); })
            | std::ranges::to<std::vector<long> >()
        );
    }

    return std::make_pair(rules, updates);
}


static std::vector<long> toposort(const std::vector<std::pair<long, long> > &rules,
                                  const std::unordered_set<long> &nodes) {
    std::unordered_map<long, long> indegree;
    std::unordered_map<long, std::vector<long> > neighbors;
    for (const auto &[left, right]: rules) {
        if (nodes.contains(left) && nodes.contains(right)) {
            indegree[right] += 1;
            neighbors[left].push_back(right);
        }
    }

    std::vector<long> out, stack;
    for (const auto node: nodes)
        if (indegree[node] == 0)
            stack.push_back(node);

    while (!stack.empty()) {
        const auto node = stack.back();
        stack.pop_back();
        out.push_back(node);
        for (const auto neighbor: neighbors[node]) {
            indegree[neighbor] -= 1;
            if (indegree[neighbor] == 0)
                stack.push_back(neighbor);
        }
    }

    return out;
}

static std::pair<bool, long> processUpdate(const std::vector<std::pair<long, long> > &rules,
                                           const std::vector<long> &updates) {
    const std::unordered_set<long> nodes(updates.begin(), updates.end());
    const auto sorted = toposort(rules, nodes);
    return std::make_pair(updates == sorted, sorted[sorted.size() / 2]);
}

namespace day05 {
    long part1(const std::string &input) {
        const auto [rules, updates] = parseInput(input);

        long sum = 0;
        for (const auto &update: updates) {
            if (const auto &[isValid, middle] = processUpdate(rules, update); isValid)
                sum += middle;
        }
        return sum;
    }

    long part2(const std::string &input) {
        auto [rules, updates] = parseInput(input);

        long sum = 0;
        for (const auto &update: updates) {
            if (const auto &[isValid, middle] = processUpdate(rules, update); !isValid)
                sum += middle;
        }
        return sum;
    }
}
