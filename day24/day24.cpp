//
// Created by Hadrian Reppas on 12/29/24.
//

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <array>
#include <map>
#include <random>
#include <ranges>
#include <print>

#include "z3++.h"

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
            default:
                assert(false);
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
        std::vector<std::pair<std::string, Node> > nodes;
        for (const auto &[name, node]: parseInput(input)) {
            if (node.op == Operation::And || node.op == Operation::Or || node.op == Operation::Xor)
                nodes.emplace_back(name, node);
        }

        z3::context ctx;
        z3::solver solver{ctx};
        std::map<std::pair<std::string, std::string>, z3::expr> swaps;
        for (const auto &v1: std::ranges::views::keys(nodes)) {
            for (const auto &v2: std::ranges::views::keys(nodes)) {
                const auto swapVar = ctx.bool_const(std::format("swap_{}_{}", v1, v2).c_str());
                swaps.insert({{v1, v2}, swapVar});
                if (v1 != v2 && swaps.contains({v2, v1}))
                    solver.add(swaps.at({v1, v2}) == swaps.at({v2, v1}));
            }
        }

        z3::expr_vector diagSwapVars{ctx};
        std::vector<int> ones;
        for (const auto &v: std::ranges::views::keys(nodes)) {
            diagSwapVars.push_back(swaps.at({v, v}));
            ones.push_back(1);
        }
        solver.add(pbeq(diagSwapVars, ones.data(), static_cast<int>(nodes.size()) - 8));

        for (const auto &v1: std::ranges::views::keys(nodes)) {
            z3::expr_vector swapVars{ctx};
            for (const auto &v2: std::ranges::views::keys(nodes))
                swapVars.push_back(swaps.at({v1, v2}));
            solver.add(pbeq(swapVars, ones.data(), 1));
        }

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<long> dist(0, 35184372088831L);

        for (long t = 0; t < 20; t++) {
            const auto x = dist(gen);
            const auto y = dist(gen);
            const auto z = x + y;

            std::unordered_map<std::string, z3::expr> before, after;
            for (const auto &v: std::ranges::views::keys(nodes)) {
                before.emplace(v, ctx.bool_const(std::format("before_{}_{}", v, t).c_str()));
                after.emplace(v, ctx.bool_const(std::format("after_{}_{}", v, t).c_str()));
            }

            for (long i = 0; i < 45; i++) {
                after.emplace(std::format("x{:02d}", i), ctx.bool_val((x >> i) & 1));
                after.emplace(std::format("y{:02d}", i), ctx.bool_val((y >> i) & 1));
            }

            for (long i = 0; i < 46; i++)
                solver.add(after.at(std::format("z{:02d}", i)) == ctx.bool_val((z >> i) & 1));

            for (const auto &[v, node]: nodes) {
                if (node.op == Operation::And)
                    solver.add((after.at(node.lhs) & after.at(node.rhs)) == before.at(v));
                else if (node.op == Operation::Or)
                    solver.add((after.at(node.lhs) | after.at(node.rhs)) == before.at(v));
                else
                    solver.add((after.at(node.lhs) ^ after.at(node.rhs)) == before.at(v));

                for (const auto &o: std::ranges::views::keys(nodes))
                    solver.add(implies(swaps.at({v, o}), before.at(v) == after.at(o)));
            }
        }

        const auto res = solver.check();
        assert(res == z3::sat);
        const auto model = solver.get_model();
        std::vector<std::string> wires;
        for (const auto &[pair, swapVar]: swaps) {
            const auto &[v1, v2] = pair;
            if (v1 < v2 && model.eval(swapVar).is_true()) {
                wires.push_back(v1);
                wires.push_back(v2);
            }
        }
        std::sort(wires.begin(), wires.end());

        std::string out;
        for (const auto &wire: wires) {
            if (!out.empty())
                out += ",";
            out += wire;
        }
        return out;
    }
}
