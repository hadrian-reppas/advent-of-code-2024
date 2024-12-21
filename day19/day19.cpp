//
// Created by Hadrian Reppas on 12/20/24.
//

#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <print>
#include <unordered_map>
#include <numeric>

#include "../util/util.h"

struct SpanHash {
    std::size_t operator()(const std::span<long> &span) const {
        return std::accumulate(span.begin(), span.end(), 0UL, [](std::size_t hash, long val) {
            return hash * 31 + std::hash<long>{}(val);
        });
    }
};

struct SpanEqual {
    bool operator()(const std::span<long> &lhs, const std::span<long> &rhs) const {
        return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }
};

static void println(const std::span<long> &towel) {
    std::print("[");
    for (long i = 0; i < towel.size(); ++i) {
        if (i > 0)
            std::print(", ");
        std::print("{}", towel[i]);
    }
    std::println("]");
}

static long parseColor(const char c) {
    switch (c) {
        case 'w': return 0;
        case 'u': return 1;
        case 'b': return 2;
        case 'r': return 3;
        case 'g': return 4;
        default: return -1;
    }
}

static std::vector<long> parseTowel(const std::string &part) {
    std::vector<long> towel;
    for (const auto c: part) {
        const auto color = parseColor(c);
        if (color != -1)
            towel.push_back(color);
    }
    return towel;
}

struct Trie {
    std::optional<std::vector<long> > towel;
    std::array<std::unique_ptr<Trie>, 5> children;

    explicit Trie() : towel{std::nullopt}, children{std::array<std::unique_ptr<Trie>, 5>{}} {
    }

    void insert(const std::vector<long> &full, const std::span<long> suffix) {
        if (suffix.empty()) {
            towel = std::make_optional(full);
            return;
        }

        const auto color = suffix[0];
        if (children[color] == nullptr)
            children[color] = std::make_unique<Trie>();
        children[color]->insert(full, suffix.subspan(1));
    }

    void get(const std::span<long> suffix, std::vector<std::span<long> > &result) {
        if (towel.has_value())
            result.emplace_back(towel.value());
        if (!suffix.empty() && children[suffix[0]] != nullptr)
            children[suffix[0]]->get(suffix.subspan(1), result);
    }
};

static std::pair<Trie, std::vector<std::vector<long> > > parseInput(
    const std::string &input) {
    const auto lines = split(input);
    auto towels = split(lines[0], ',')
                  | std::ranges::views::transform(parseTowel)
                  | std::ranges::to<std::vector<std::vector<long> > >();
    Trie trie{};
    for (auto &towel: towels)
        trie.insert(towel, towel);
    std::vector<std::vector<long> > patterns;
    for (long i = 2; i < lines.size(); ++i)
        patterns.push_back(parseTowel(lines[i]));
    return std::make_tuple(std::move(trie), patterns);
}

static long countArrangements(const std::span<long> pattern, Trie &trie,
                              std::unordered_map<std::span<long>, long, SpanHash, SpanEqual> &cache) {
    if (pattern.empty())
        return 1;
    if (cache.contains(pattern))
        return cache[pattern];

    std::vector<std::span<long> > prefixes;
    trie.get(pattern, prefixes);
    std::reverse(prefixes.begin(), prefixes.end());

    long count = 0;
    for (const auto &prefix: prefixes)
        count += countArrangements(pattern.subspan(prefix.size()), trie, cache);
    return cache[pattern] = count;
}

namespace day19 {
    long part1(const std::string &input) {
        auto [trie, patterns] = parseInput(input);

        long count = 0;
        std::unordered_map<std::span<long>, long, SpanHash, SpanEqual> cache;
        for (auto &pattern: patterns)
            count += countArrangements(pattern, trie, cache) > 0;
        return count;
    }

    long part2(const std::string &input) {
        auto [trie, patterns] = parseInput(input);

        long count = 0;
        std::unordered_map<std::span<long>, long, SpanHash, SpanEqual> cache;
        for (auto &pattern: patterns)
            count += countArrangements(pattern, trie, cache);
        return count;
    }
}
