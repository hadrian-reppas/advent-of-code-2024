//
// Created by Hadrian Reppas on 12/14/24.
//

#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <set>

struct File {
    long start;
    long size;
    long id;
};

class FileSystem {
    std::array<std::set<long>, 10> free;
    std::vector<File> files;

public:
    explicit FileSystem(const std::string &input) {
        long pos = 0;
        for (long i = 0; i < input.size(); i++) {
            if (input[i] == '0') continue;
            if (i % 2 == 0)
                files.emplace_back(pos, input[i] - '0', i / 2);
            else
                free[input[i] - '0'].insert(pos);
            pos += input[i] - '0';
        }
    }

    std::optional<long> findInterval(const long size, const long start) {
        long minStart = std::numeric_limits<long>::max(), index = -1;
        for (long i = size; i < 10; i++) {
            if (!free[i].empty() && *free[i].begin() < minStart) {
                minStart = *free[i].begin();
                index = i;
            }
        }

        if (minStart >= start)
            return std::nullopt;

        free[index].erase(free[index].begin());
        if (index > size)
            free[index - size].insert(minStart + size);
        return minStart;
    }

    void compactify() {
        std::vector<File> todo;
        std::swap(files, todo);
        std::ranges::reverse(todo);

        for (auto file: todo) {
            if (const auto newStart = findInterval(file.size, file.start); newStart != std::nullopt)
                file.start = newStart.value();
            files.push_back(file);
        }
    }

    long checksum() {
        long checksum = 0;
        for (const auto &[start, size, id]: files) {
            for (long i = 0; i < size; i++)
                checksum += id * (start + i);
        }
        return checksum;
    }
};

namespace day09 {
    long part1(const std::string &input) {
        std::vector<std::pair<long, long> > fs;
        std::vector<long> free;
        long pos = 0, size = 0;
        for (long i = 0; i < input.size(); i++) {
            if (i % 2 == 0) {
                size += input[i] - '0';
                for (long j = 0; j < input[i] - '0'; j++)
                    fs.emplace_back(pos++, i / 2);
            } else {
                for (long j = 0; j < input[i] - '0'; j++)
                    free.push_back(pos++);
            }
        }

        std::ranges::reverse(free);
        std::vector<long> compacted(size, -1);
        while (free.back() < fs.back().first) {
            compacted[free.back()] = fs.back().second;
            free.pop_back();
            fs.pop_back();
        }
        for (const auto &[index, file]: fs) {
            compacted[index] = file;
        }

        long checksum = 0;
        for (long i = 0; i < size; i++)
            checksum += i * compacted[i];
        return checksum;
    }

    long part2(const std::string &input) {
        FileSystem fs{input};
        fs.compactify();
        return fs.checksum();
    }
}
