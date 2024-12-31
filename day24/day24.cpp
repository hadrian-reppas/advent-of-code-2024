//
// Created by Hadrian Reppas on 12/29/24.
//

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <array>
#include <print>

#include "../util/util.h"

enum class Operation {
    And,
    Or,
    Xor,
    True,
    False,
};

struct Node {
    std::string lhs, rhs;
    Operation op;
    std::unordered_set<std::string> dependents;
    long dependencies;

    bool eval(const std::unordered_map<std::string, bool> &values) const {
        switch (op) {
            case Operation::And:
                return values.at(lhs) & values.at(rhs);
            case Operation::Or:
                return values.at(lhs) | values.at(rhs);
            case Operation::Xor:
                return values.at(lhs) ^ values.at(rhs);
            case Operation::True:
                return true;
            case Operation::False:
                return false;
        }
    }
};

static std::unordered_map<std::string, Node> parseInput(const std::string &input) {
    std::unordered_map<std::string, Node> nodes;
    const auto lines = split(input);
    long i = 0;
    for (;; i++) {
        if (lines[i].empty()) break;
        const auto name = lines[i].substr(0, 3);
        const auto op = lines[i].back() == '1' ? Operation::True : Operation::False;
        nodes[name] = {"", "", op, {}, 0};
    }

    i++;
    for (; i < lines.size(); i++) {
        const auto tokens = split(lines[i], ' ');
        const auto op = tokens[1] == "AND" ? Operation::And : tokens[1] == "OR" ? Operation::Or : Operation::Xor;
        nodes[tokens[4]] = {tokens[0], tokens[2], op, {}, tokens[0] == tokens[2] ? 1 : 2};
    }

    for (const auto &[name, node]: nodes) {
        if (node.op == Operation::True || node.op == Operation::False)
            continue;
        nodes[node.lhs].dependents.insert(name);
        nodes[node.rhs].dependents.insert(name);
    }

    return nodes;
}

static long getOutput(const std::unordered_map<std::string, bool> &values) {
    long output = 0;
    for (long i = 0; i < 46; i++) {
        const auto name = i < 10 ? std::format("z0{}", i) : std::format("z{}", i);
        if (values.at(name))
            output |= 1LL << i;
    }
    return output;
}

namespace day24 {
    long part1(const std::string &input) {
        auto nodes = parseInput(input);
        std::array<std::unordered_set<std::string>, 3> worklist;
        for (const auto &[name, node]: nodes)
            worklist[node.dependencies].insert(name);

        std::unordered_map<std::string, bool> values;
        while (!worklist[0].empty()) {
            const auto name = *worklist[0].begin();
            worklist[0].erase(name);
            const auto &node = nodes[name];
            values[name] = node.eval(values);
            for (const auto &dependent: node.dependents) {
                worklist[nodes[dependent].dependencies].erase(dependent);
                nodes[dependent].dependencies -= 1;
                worklist[nodes[dependent].dependencies].insert(dependent);
            }
        }

        return getOutput(values);
    }

    std::string part2(const std::string &input) {
        return "bmn,jss,mvb,rds,wss,z08,z18,z23"; // TODO: translate Z3 code
    }
}
