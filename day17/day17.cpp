//
// Created by Hadrian Reppas on 12/17/24.
//

#include <ranges>
#include <string>
#include <vector>
#include <cmath>

#include "z3++.h"

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

struct Translator {
    z3::context ctx;
    z3::optimize opt;
    z3::sort bv_sort;
    z3::expr a, a0, b, c;
    std::vector<long> ops;
    long an, bn, cn;

    explicit Translator(const std::string &input)
        : opt{ctx},
          bv_sort{ctx.bv_sort(64)},
          a{ctx.constant("a0", bv_sort)},
          a0{a},
          b{ctx.constant("b0", bv_sort)},
          c{ctx.constant("c0", bv_sort)},
          an{0},
          bn{0},
          cn{0} {
        ops = Computer{input}.ops;
    }

    z3::expr constant(const long arg) {
        return ctx.bv_val(static_cast<int>(arg), 64);
    }

    z3::expr combo(const long arg) {
        if (arg <= 3)
            return constant(arg);
        if (arg == 4)
            return a;
        if (arg == 5)
            return b;
        if (arg == 6)
            return c;
        throw std::invalid_argument("invalid argument");
    }

    z3::expr freshA() {
        return a = ctx.constant(std::format("a{}", ++an).c_str(), bv_sort);
    }

    z3::expr freshB() {
        return b = ctx.constant(std::format("b{}", ++bn).c_str(), bv_sort);
    }

    z3::expr freshC() {
        return c = ctx.constant(std::format("c{}", ++cn).c_str(), bv_sort);
    }

    void compilePass(const long out, const bool last) {
        bool seenOut = false;
        for (long i = 0; i < ops.size() - 2; i += 2) {
            const auto op = ops[i];
            const auto arg = ops[i + 1];

            switch (op) {
                case 1: {
                    const auto bb = b ^ constant(arg);
                    opt.add(freshB() == bb);
                    break;
                }
                case 2: {
                    const auto bb = combo(arg) & constant(7);
                    opt.add(freshB() == bb);
                    break;
                }
                case 4: {
                    const auto bb = b ^ c;
                    opt.add(freshB() == bb);
                    break;
                }
                case 5: {
                    assert(!seenOut); // Only see one out instruction
                    seenOut = true;
                    const auto o = combo(arg) & constant(7);
                    opt.add(o == constant(out));
                    break;
                }
                case 0:
                case 6:
                case 7: {
                    const auto res = lshr(a, combo(arg));
                    if (op == 0)
                        opt.add(freshA() == res);
                    else if (op == 6)
                        opt.add(freshB() == res);
                    else
                        opt.add(freshC() == res);
                    break;
                }
                default:
                    assert(false);
            }
        }

        assert(ops[ops.size() - 2] == 3); // Last instruction is jnz
        assert(ops[ops.size() - 1] == 0); // Jump goes to 0
        if (last)
            opt.add(a == constant(0));
        else
            opt.add(a != constant(0));
    }


    void compile() {
        for (long i = 0; i < ops.size(); i++)
            compilePass(ops[i], i == ops.size() - 1);
    }
};

namespace day17 {
    std::string part1(const std::string &input) {
        Computer computer{input};
        while (computer.step());
        return computer.output();
    }

    long part2(const std::string &input) {
        Translator translator{input};
        translator.compile();

        translator.opt.minimize(translator.a0);
        const auto res = translator.opt.check();
        assert(res == z3::sat);
        const z3::model model = translator.opt.get_model();
        return model.eval(translator.a0).as_int64();
    }
}
