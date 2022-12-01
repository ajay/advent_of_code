#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::pair<std::string, size_t>> readFile(const std::string& filename = "input.txt") {
    std::vector<std::pair<std::string, size_t>> data{};

    std::ifstream file{filename};
    while (file) {
        std::string direction{};
        size_t value{};
        file >> direction;
        file >> value;
        data.emplace_back(direction, value);
    }

    return data;
}

size_t part1() {
    const auto data = readFile();

    size_t depth = 0;
    size_t horizontal = 0;
    for (const auto& [direction, value] : data) {
        if (direction == "forward") {
            horizontal += value;
        } else if (direction == "down") {
            depth += value;
        } else if (direction == "up") {
            depth -= value;
        }
    }

    return depth * horizontal;
}

size_t part2() {
    const auto data = readFile();

    size_t depth = 0;
    size_t horizontal = 0;
    ssize_t aim = 0;
    for (const auto& [direction, value] : data) {
        if (direction == "forward") {
            horizontal += value;
            depth += value * aim;
        } else if (direction == "down") {
            aim += value;
        } else if (direction == "up") {
            aim -= value;
        }
    }

    return depth * horizontal;
}

int main() {
    std::cout << "part1: " << part1() << '\n';
    std::cout << "part2: " << part2() << '\n';
}
