//
// Created by Hadrian Reppas on 12/2/24.
//

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

#include "../util/util.h"

static std::vector<std::vector<long> > parseInput(const std::string &input) {
    std::vector<std::vector<long> > out;
    for (const auto &line: split(input)) {
        out.push_back(
            split(line, ' ')
            | std::ranges::views::transform([](const auto &c) { return std::stol(c); })
            | std::ranges::to<std::vector<long> >());
    }
    return out;
}

static bool isValid(const std::vector<long> &report) {
    if (report[1] > report[0]) {
        for (size_t i = 0; i < report.size() - 1; i++)
            if (report[i + 1] < report[i] + 1 || report[i + 1] > report[i] + 3)
                return false;
    } else {
        for (size_t i = 0; i < report.size() - 1; i++)
            if (report[i + 1] > report[i] - 1 || report[i + 1] < report[i] - 3)
                return false;
    }
    return true;
}

static bool isValidPart2(std::vector<long> &report) {
    for (size_t i = 0; i < report.size(); i++) {
        const long value = report[i];
        report.erase(report.begin() + i);
        const bool valid = isValid(report);
        report.insert(report.begin() + i, value);
        if (valid)
            return true;
    }
    return false;
}

namespace day02 {
    long part1(const std::string &input) {
        auto reports = parseInput(input);
        return std::ranges::count_if(reports, isValid);
    }

    long part2(const std::string &input) {
        auto reports = parseInput(input);
        return std::ranges::count_if(reports, isValidPart2);
    }
}
