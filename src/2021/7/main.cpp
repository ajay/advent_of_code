#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

std::string readFile(const std::string& filename = "input.txt") {
    std::ifstream file{filename};
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
    std::vector<std::string> v{};
    size_t start = 0;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        v.push_back(str.substr(start, end - start));
    }

    return v;
}

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec) {
    std::vector<size_t> numVec{};

    std::transform(strVec.begin(), strVec.end(), std::back_inserter(numVec), [](const auto& s) {
        return std::stoi(s);
    });

    return numVec;
}

std::pair<size_t, size_t> strVecToPair(const std::vector<std::string>& strVec) {
    const auto v = strVecToNumVec(strVec);
    assert(v.size() == 2);
    return {v[0], v[1]};
}

template<class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
    std::stringstream s{};

    s << "{\n";
    for (const auto& [key, value] : m) {
        s << "  { " << key << ", " << value << " },\n";
    }
    s << "}\n";

    return os << s.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& p) {
    const auto& [one, two] = p;

    std::stringstream s{};
    s << "{ " << one << ", " << two << " }";

    return os << s.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    constexpr auto delim = ' ';
    std::stringstream s{};

    s << '{' << delim;
    for (const auto& e : v) {
        s << e << delim;
    }
    s << '}';

    return os << s.str();
}

std::vector<size_t> parseFile() {
    const auto raw = readFile();
    const auto lines = split(raw, ",");
    const auto nums = strVecToNumVec(lines);

    return nums;
}

size_t part1(bool increasingCost = false) {
    const auto crabs = parseFile();
    const auto maxPosition = *std::max_element(crabs.begin(), crabs.end());

    std::unordered_map<size_t, size_t> positions{};
    for (size_t i = 0; i <= maxPosition; ++i) {
        positions.emplace(i, 0);
    }

    for (auto& [position, sum] : positions) {
        size_t delta = 0;
        for (const auto& crab : crabs) {
            const auto diff =
                static_cast<size_t>(std::abs(static_cast<ssize_t>(crab) - static_cast<ssize_t>(position)));

            if (!increasingCost) {
                delta += diff;
            } else {
                for (size_t i = 0; i <= diff; ++i) {
                    delta += i;
                }
            }
        }

        sum = delta;
    }

    // std::cout << positions << '\n';

    return std::min_element(positions.begin(), positions.end(),
                            [](const auto& lhs, const auto& rhs) {
                                return lhs.second < rhs.second;
                            })
        ->second;
}

size_t part2() {
    return part1(true);
}

int main() {
    const auto p1 = part1();
    assert(p1 == 352254);
    std::cout << "part1: " << p1 << '\n';

    const auto p2 = part2();
    assert(p2 == 99053143);
    std::cout << "part2: " << p2 << '\n';
}
