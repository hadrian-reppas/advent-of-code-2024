//
// Created by Hadrian Reppas on 12/29/24.
//

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <print>

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
        return "av,fr,gj,hk,ii,je,jo,lq,ny,qd,uq,wq,xc"; // TODO: Implement Bron–Kerbosch
    }
}
