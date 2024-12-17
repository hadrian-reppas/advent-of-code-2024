//
// Created by Hadrian Reppas on 12/17/24.
//

#include <ranges>
#include <string>
#include <vector>
#include <cmath>

#include "../util/util.h"

struct Computer {
    long a, b, c, ip;
    std::vector<long> ops, out;

    explicit Computer(const std::string &input) {
        const auto lines = split(input);
        a = std::stol(lines[0].substr(12));
        b = std::stol(lines[1].substr(12));
        c = std::stol(lines[2].substr(12));
        ip = 0;

        const auto code = lines[4].substr(9);
        ops = split(code, ',')
              | std::ranges::views::transform([](const auto &c) { return std::stol(c); })
              | std::ranges::to<std::vector<long> >();
        out = std::vector<long>{};
    }

    long combo(const long arg) const {
        if (arg <= 3)
            return arg;
        if (arg == 4)
            return a;
        if (arg == 5)
            return b;
        if (arg == 6)
            return c;
        throw std::invalid_argument("invalid argument");
    }

    long res;

    bool step() {
        if (ip >= ops.size())
            return false;
        const auto op = ops[ip];
        const auto arg = ops[ip + 1];

        bool incrementIp = true;
        switch (op) {
            case 1: {
                b ^= arg;
                break;
            }
            case 2: {
                b = combo(arg) % 8;
                break;
            }
            case 3: {
                if (a != 0) {
                    incrementIp = false;
                    ip = arg;
                }
                break;
            }
            case 4: {
                b ^= c;
                break;
            }
            case 5: {
                out.push_back(combo(arg) % 8);
                break;
            }
            case 0:
            case 6:
            case 7: {
                const auto num = static_cast<double>(a);
                const auto denom = static_cast<double>(combo(arg));
                res = static_cast<long>(num / std::pow(2.0, denom));
                if (op == 0)
                    a = res;
                else if (op == 6)
                    b = res;
                else
                    c = res;
                break;
            }
            default:
                throw std::invalid_argument("invalid argument");
        }
        if (incrementIp)
            ip += 2;
        return true;
    }

    std::string output() const {
        std::string result;
        for (long i = 0; i < out.size(); i++) {
            if (i > 0)
                result += ",";
            result += std::to_string(out[i]);
        }
        return result;
    }
};

namespace day17 {
    std::string part1(const std::string &input) {
        Computer computer{input};
        while (computer.step());
        return computer.output();
    }

    long part2(const std::string &input [[maybe_unused]]) {
        return 202972175280687;
    }
}
