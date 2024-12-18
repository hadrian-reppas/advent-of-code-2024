//
// Created by Hadrian Reppas on 12/18/24.
//

#include <format>
#include <string>
#include <vector>
#include <set>
#include <ranges>
#include <queue>

#include "../util/util.h"

static std::array<std::pair<long, long>, 4> deltas = {
    std::make_pair(0, 1),
    {1, 0},
    {0, -1},
    {-1, 0},
};

static std::vector<std::pair<long, long> > parseInput(const std::string &input) {
    return split(input)
           | std::ranges::views::transform([](const auto &line) {
               const auto parts = split(line, ',');
               return std::make_pair(std::stol(parts[0]), std::stol(parts[1]));
           })
           | std::ranges::to<std::vector<std::pair<long, long> > >();
}

static std::optional<long> distance(const std::vector<std::pair<long, long> > &bytes, const long count) {
    const std::set<std::pair<long, long> > corrupted(bytes.begin(), bytes.begin() + count);

    std::set<std::pair<long, long> > seen;
    std::queue<std::tuple<long, long, long> > queue;
    queue.emplace(0, 0, 0);

    while (!queue.empty()) {
        const auto [x, y, d] = queue.front();
        queue.pop();

        if (x == 70 && y == 70)
            return std::make_optional(d);
        if (seen.contains({x, y}))
            continue;
        seen.insert({x, y});

        for (const auto &[dx, dy]: deltas) {
            const auto xn = x + dx, yn = y + dy;
            if (xn >= 0 && xn <= 70 && yn >= 0 && yn <= 70 && !corrupted.contains({xn, yn}))
                queue.emplace(xn, yn, d + 1);
        }
    }

    return std::nullopt;
}

namespace day18 {
    long part1(const std::string &input) {
        const auto bytes = parseInput(input);
        return distance(bytes, 1024).value();
    }

    std::string part2(const std::string &input) {
        const auto bytes = parseInput(input);
        long low = 1024, high = static_cast<long>(bytes.size()), highestUnreachable = high;
        while (low < high) {
            const auto mid = (low + high) / 2;
            if (distance(bytes, mid).has_value()) {
                low = mid + 1;
            } else {
                highestUnreachable = mid;
                high = mid;
            }
        }

        const auto [x, y] = bytes[highestUnreachable - 1];
        return std::format("{},{}", x, y);
    }
}
