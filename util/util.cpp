//
// Created by Hadrian Reppas on 12/6/24.
//

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &input, const char delim = '\n') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(input);
    while (std::getline(stream, token, delim))
        tokens.push_back(token);
    return tokens;
}
