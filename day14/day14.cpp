//
// Created by Hadrian Reppas on 12/15/24.
//

#include <string>
#include <vector>
#include <ranges>
#include <print>

#include "../util/util.h"

static long mod(const long a, const long b) {
    return (a % b + b) % b;
}

struct Robot {
    long x, y;
    long vx, vy;

    void update(const long t) {
        x = mod(x + t * vx, 101);
        y = mod(y + t * vy, 103);
    }

    std::optional<long> quadrant() const {
        if (x < 50 && y < 51)
            return std::make_optional(0);
        if (x < 50 && y > 51)
            return std::make_optional(1);
        if (x > 50 && y < 51)
            return std::make_optional(2);
        if (x > 50 && y > 51)
            return std::make_optional(3);
        return std::nullopt;
    }
};

static std::pair<long, long> parseTuple(const std::string &tuple) {
    const auto comma = tuple.find(',');
    return std::make_pair(
        std::stol(tuple.substr(2, comma - 2)),
        std::stol(tuple.substr(comma + 1))
    );
}

static Robot parseRobot(const std::string &line) {
    const auto tuples = split(line, ' ');
    const auto [x, y] = parseTuple(tuples[0]);
    const auto [vx, vy] = parseTuple(tuples[1]);
    return {x, y, vx, vy};
}

static std::vector<Robot> parseInput(const std::string &input) {
    const auto lines = split(input);
    return lines | std::views::transform(parseRobot) | std::ranges::to<std::vector<Robot> >();
}

static float stdev(const std::vector<Robot> &robots) {
    float xMean = 0.0, yMean = 0.0;
    for (auto &robot: robots) {
        xMean += static_cast<float>(robot.x);
        yMean += static_cast<float>(robot.y);
    }
    xMean /= static_cast<float>(robots.size());
    yMean /= static_cast<float>(robots.size());

    float total = 0.0;
    for (const auto &robot: robots) {
        const float dx = static_cast<float>(robot.x) - xMean;
        const float dy = static_cast<float>(robot.y) - yMean;
        total += dx * dx + dy * dy;
    }
    return total / static_cast<float>(robots.size());
}

namespace day14 {
    long part1(const std::string &input) {
        std::array<long, 4> counts = {};
        auto robots = parseInput(input);
        for (auto &robot: robots) {
            robot.update(100);
            const auto quadrant = robot.quadrant();
            if (quadrant.has_value())
                ++counts[quadrant.value()];
        }
        return counts[0] * counts[1] * counts[2] * counts[3];
    }

    long part2(const std::string &input) {
        auto robots = parseInput(input);
        for (long i = 1; ; i++) {
            for (auto &robot: robots)
                robot.update(1);

            if (stdev(robots) < 1000.0)
                return i;
        }
    }
}
