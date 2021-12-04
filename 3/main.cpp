#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

template<class T = size_t>
std::vector<T> readFile(const std::string& filename = "input.txt") {
    std::vector<T> data{};

    std::ifstream file{filename};
    while (file) {
        std::string element{};
        file >> element;
        if (!element.empty()) {
            T d = std::stoi(element, 0, 2);
            data.emplace_back(d);
        }
    }

    return data;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    constexpr auto delim = "\n";
    std::stringstream s{};

    s << '{' << delim;
    for (const auto& e : v) {
        s << std::bitset<12>(e) << delim;
    }
    s << '}';

    return os << s.str();
}

size_t part1() {
    constexpr size_t kNumBits = 12;
    const auto data = readFile();

    std::vector<size_t> bits(kNumBits);
    for (size_t bit = 0; bit < bits.size(); ++bit) {
        for (const auto& num : data) {
            bits[bit] += (num & (1 << bit)) ? 1 : 0;
        }
    }

    size_t gamma = 0;
    for (size_t bit = 0; bit < bits.size(); ++bit) {
        if ((bits[bit] > (data.size() / 2))) {
            gamma |= (1 << bit);
        }
    }

    const size_t epsilon = (~gamma) & ((1 << kNumBits) - 1);

    // std::cout << std::bitset<kNumBits>(gamma) << '\n';
    // std::cout << std::bitset<kNumBits>(epsilon) << '\n';

    return gamma * epsilon;
}

size_t part2() {
    constexpr size_t kNumBits = 12;
    const auto data = readFile();

    auto nums = data;
    for (size_t bit = (kNumBits - 1); (nums.size() > 1) && (bit >= 0); --bit) {
        const size_t ones = std::count_if(nums.begin(), nums.end(), [&bit](const auto& n) {
            return n & (1 << bit);
        });

        const bool one = (ones >= (nums.size() / 2));
        std::erase_if(nums, [&](const auto& n) {
            const auto b = (n & (1 << bit));
            return one ? !b : b;
        });
    }
    assert(nums.size() == 1);
    const size_t oxygen = nums[0];

    nums = data;
    for (size_t bit = (kNumBits - 1); (nums.size() > 1) && (bit >= 0); --bit) {
        const size_t ones = std::count_if(nums.begin(), nums.end(), [&bit](const auto& n) {
            return (n & (1 << bit));
        });

        const bool one = (ones >= (nums.size() / 2));
        std::erase_if(nums, [&](const auto& n) {
            const auto b = (n & (1 << bit));
            return one ? b : !b;
        });
    }
    assert(nums.size() == 1);
    const size_t co2 = nums[0];

    // std::cout << std::bitset<kNumBits>(oxygen) << ' ' << oxygen << '\n';
    // std::cout << std::bitset<kNumBits>(co2) << ' ' << co2 << '\n';

    return oxygen * co2;
}

int main() {
    std::cout << "part1: " << part1() << '\n';
    std::cout << "part2: " << part2() << '\n';
}
