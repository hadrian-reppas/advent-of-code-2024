//
// Created by Hadrian Reppas on 12/29/24.
//

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <set>
#include <array>

#include "../util/util.h"

static std::unordered_map<std::string, std::unordered_set<std::string> > parseInput(const std::string &input) {
    std::unordered_map<std::string, std::unordered_set<std::string> > graph;
    for (const auto &line: split(input)) {
        const auto left = line.substr(0, 2);
        const auto right = line.substr(3);
        graph[left].insert(right);
        graph[right].insert(left);
    }
    return graph;
}

static std::vector<std::array<std::string, 3> > getTriangles(
    const std::unordered_map<std::string, std::unordered_set<std::string> > &graph) {
    std::vector<std::array<std::string, 3> > triangles;
    for (const auto &[a, neighbors]: graph) {
        for (const auto &b: neighbors) {
            if (b <= a) continue;
            for (const auto &c: neighbors) {
                if (c <= b) continue;
                if (graph.at(b).contains(c))
                    triangles.push_back({a, b, c});
            }
        }
    }
    return triangles;
}

static std::unordered_set<std::string_view> intersection(const std::unordered_set<std::string_view> &a,
                                                         const std::unordered_set<std::string_view> &b) {
    std::unordered_set<std::string_view> result;
    for (const auto &s: a) {
        if (b.contains(s))
            result.insert(s);
    }
    return result;
}

static std::unordered_set<std::string_view> difference(const std::unordered_set<std::string_view> &a,
                                                       const std::unordered_set<std::string_view> &b) {
    std::unordered_set<std::string_view> result;
    for (const auto &s: a) {
        if (!b.contains(s))
            result.insert(s);
    }
    return result;
}

static std::unordered_set<std::string_view> with(const std::unordered_set<std::string_view> &x,
                                                 const std::string_view s) {
    std::unordered_set<std::string_view> result = x;
    result.insert(s);
    return result;
}

static std::string_view sample(const std::unordered_set<std::string_view> &p,
                               const std::unordered_set<std::string_view> &x) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<> prob_dist(0, 1);
    const auto prob = static_cast<double>(p.size()) / static_cast<double>(p.size() + x.size());
    const auto fromP = prob_dist(gen) < prob;

    const auto &set = fromP ? p : x;

    const std::vector<std::string_view> elements(set.begin(), set.end());
    std::uniform_int_distribution<> index_dist(0, static_cast<int>(elements.size() - 1));
    return elements[index_dist(gen)];
}

static void search(
    const std::unordered_map<std::string_view, std::unordered_set<std::string_view> > &graph,
    const std::unordered_set<std::string_view> &r,
    const std::unordered_set<std::string_view> &p,
    const std::unordered_set<std::string_view> &x,
    std::vector<std::unordered_set<std::string_view> > &out) {
    if (p.empty() && x.empty()) {
        out.push_back(r);
        return;
    }

    auto pMut = p;
    auto xMut = x;
    const auto u = sample(p, x);
    for (const auto v: difference(p, graph.at(u))) {
        search(graph, with(r, v), intersection(p, graph.at(v)), intersection(x, graph.at(v)), out);
        pMut.erase(v);
        xMut.insert(v);
    }
}

static std::unordered_set<std::string_view> maxClique(
    const std::unordered_map<std::string, std::unordered_set<std::string> > &rawGraph) {
    std::unordered_map<std::string_view, std::unordered_set<std::string_view> > graph;
    std::unordered_set<std::string_view> r, p, x;
    for (const auto &[node, neighbors]: rawGraph) {
        p.insert(node);
        for (const auto &neighbor: neighbors)
            graph[node].insert(neighbor);
    }

    std::vector<std::unordered_set<std::string_view> > out;
    search(graph, r, p, x, out);
    long maxClique = 0;
    for (long i = 1; i < out.size(); i++) {
        if (out[i].size() > out[maxClique].size())
            maxClique = i;
    }
    return out[maxClique];
}

namespace day23 {
    long part1(const std::string &input) {
        const auto graph = parseInput(input);
        const auto triangles = getTriangles(graph);
        long count = 0;
        for (const auto &triangle: triangles) {
            if (triangle[0][0] == 't' || triangle[1][0] == 't' || triangle[2][0] == 't')
                count += 1;
        }
        return count;
    }

    std::string part2(const std::string &input) {
        const auto graph = parseInput(input);
        const auto clique = maxClique(graph);
        const std::set<std::string_view> sorted(clique.begin(), clique.end());
        std::string result;
        for (const auto &node: sorted) {
            if (!result.empty())
                result += ",";
            result += node;
        }
        return result;
    }
}
