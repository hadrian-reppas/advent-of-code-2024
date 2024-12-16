//
// Created by Hadrian Reppas on 12/15/24.
//

#include <string>
#include <vector>
#include <optional>
#include <cassert>

#include "../util/util.h"

struct Game {
    long dxa, dya;
    long dxb, dyb;
    long px, py;
};

static std::pair<long, long> parsePrize(const std::string &line) {
    const auto eq = line.find('=');
    const auto comma = line.find(',');
    return std::make_pair(
        std::stoi(line.substr(eq + 1, comma - eq - 1)),
        std::stoi(line.substr(comma + 4))
    );
}

static std::pair<long, long> parseButton(const std::string &line) {
    const auto plus = line.find('+');
    const auto comma = line.find(',');
    return std::make_pair(
        std::stoi(line.substr(plus + 1, comma - plus - 1)),
        std::stoi(line.substr(comma + 4))
    );
}

static std::vector<Game> parseInput(const std::string &input) {
    auto lines = split(input);
    lines.emplace_back();
    std::vector<Game> games;
    while (!lines.empty()) {
        lines.pop_back();
        const auto [px, py] = parsePrize(lines.back());
        lines.pop_back();
        const auto [dxb, dyb] = parseButton(lines.back());
        lines.pop_back();
        const auto [dxa, dya] = parseButton(lines.back());
        lines.pop_back();
        games.emplace_back(dxa, dya, dxb, dyb, px, py);
    }
    return games;
}

static std::optional<long> solve(const Game &game) {
    const auto [dxa, dya, dxb, dyb, px, py] = game;
    const auto delta = dxa * dyb - dxb * dya;
    assert(delta != 0);

    const auto u = dyb * px - dxb * py;
    if (u % delta != 0)
        return std::nullopt;
    const auto aSteps = u / delta;
    if (aSteps < 0)
        return std::nullopt;

    const auto v = dxa * py - dya * px;
    if (v % delta != 0)
        return std::nullopt;
    const auto bSteps = v / delta;
    if (bSteps < 0)
        return std::nullopt;

    return std::make_optional(3 * aSteps + bSteps);
}

namespace day13 {
    long part1(const std::string &input) {
        const auto games = parseInput(input);
        long totalTokens = 0;
        for (const auto &game: games) {
            const auto tokens = solve(game);
            if (tokens.has_value())
                totalTokens += tokens.value();
        }
        return totalTokens;
    }

    long part2(const std::string &input) {
        auto games = parseInput(input);
        long totalTokens = 0;
        for (auto &game: games) {
            game.px += 10000000000000;
            game.py += 10000000000000;
            const auto tokens = solve(game);
            if (tokens.has_value())
                totalTokens += tokens.value();
        }
        return totalTokens;
    }
}
