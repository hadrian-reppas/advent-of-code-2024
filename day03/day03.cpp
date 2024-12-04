//
// Created by Hadrian Reppas on 12/3/24.
//

#include <string>
#include <regex>
#include <print>

static std::vector<std::string> parseInput(const std::string &input) {
    const std::regex regex("mul\\(\\d+,\\d+\\)|do\\(\\)|don't\\(\\)");
    const auto matches = std::sregex_iterator(input.begin(), input.end(), regex);
    const auto end = std::sregex_iterator();

    std::vector<std::string> out;
    for (auto i = matches; i != end; ++i) {
        out.push_back(i->str());
    }
    return out;
}

static long evaluateMul(const std::string &mul) {
    const auto comma = mul.find(',');
    const auto lhs = std::stoi(mul.substr(4, comma));
    const auto rhs = std::stoi(mul.substr(comma + 1));
    return lhs * rhs;
}

namespace day03 {
    void part1(const std::string &input) {
        const auto commands = parseInput(input);

        long sum = 0;
        for (const auto &command: commands) {
            if (command.starts_with("mul")) {
                sum += evaluateMul(command);
            }
        }

        std::println("{}", sum);
    }

    void part2(const std::string &input) {
        const auto commands = parseInput(input);

        long sum = 0;
        bool active = true;
        for (const auto &command: commands) {
            if (command.starts_with("don't")) {
                active = false;
            } else if (command.starts_with("do")) {
                active = true;
            } else {
                if (active)
                    sum += evaluateMul(command);
            }
        }

        std::println("{}", sum);
    }
}
