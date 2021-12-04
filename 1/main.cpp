#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

template<class T = size_t>
std::vector<T> readFile(const std::string& filename = "input.txt") {
    std::vector<T> data{};

    std::ifstream file{filename};
    while (file) {
        T element{};
        file >> element;
        data.emplace_back(element);
    }

    return data;
}

size_t part1() {
    const auto data = readFile();

    size_t increases = 0;
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] > data[i - 1]) {
            ++increases;
        }
    }

    return increases;
}

size_t part2() {
    const auto data = readFile();

    size_t increases = 0;
    for (size_t i = 1; i < data.size() - 2; ++i) {
        const size_t sum1 = data[i - 1] + data[i] + data[i + 1];
        const size_t sum2 = data[i] + data[i + 1] + data[i + 2];

        if (sum2 > sum1) {
            ++increases;
        }
    }

    return increases;
}

int main() {
    std::cout << "part1: " << part1() << '\n';
    std::cout << "part2: " << part2() << '\n';
}
