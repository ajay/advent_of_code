#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// printing

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

template<class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v);

template<class P1, class P2>
std::ostream& operator<<(std::ostream& os, const std::pair<P1, P2>& p);

template<class T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& o) {
    if (o) {
        os << *o;
    } else {
        os << "std::nullopt";
    }

    return os;
}

template<class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    std::stringstream s{};

    s << "{\n";
    for (const auto& [key, value] : m) {
        s << "  { " << key << ", " << value << " },\n";
    }
    s << "}\n";

    return os << s.str();
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

template<class P1, class P2>
std::ostream& operator<<(std::ostream& os, const std::pair<P1, P2>& p) {
    const auto& [one, two] = p;

    std::stringstream s{};
    s << "{ " << one << ", " << two << " }\n";

    return os << s.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v) {
    constexpr auto delim = is_specialization<T, std::set>::value ? '\n' : ' ';
    std::stringstream s{};

    s << '{' << delim;
    for (const auto& e : v) {
        s << e << delim;
    }
    s << '}';

    return os << s.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    constexpr auto delim = is_specialization<T, std::vector>::value ? '\n' : ' ';
    std::stringstream s{};

    s << '{' << delim;
    for (const auto& e : v) {
        s << e << delim;
    }
    s << '}';

    return os << s.str();
}

////////////////////////////////////////////////////////////////////////////////
// file IO / string parsing

std::string readFile(const std::string& filename = "input.txt") {
    std::ifstream file{filename};
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
    std::vector<std::string> v{};
    size_t begin = 0;
    size_t end = 0;

    while ((begin = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, begin);
        v.push_back(str.substr(begin, end - begin));
    }

    return v;
}

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec) {
    std::vector<size_t> numVec{};

    std::transform(strVec.begin(), strVec.end(), std::back_inserter(numVec), [](const std::string& s) {
        return std::stoul(s);
    });

    return numVec;
}

std::pair<std::string, std::string> strVecToStrPair(const std::vector<std::string>& strVec) {
    assert(strVec.size() == 2);
    return {strVec[0], strVec[1]};
}

std::pair<size_t, size_t> strVecToNumPair(const std::vector<std::string>& strVec) {
    const auto v = strVecToNumVec(strVec);
    assert(v.size() == 2);
    return {v[0], v[1]};
}

std::vector<size_t> strToNumVec(const std::string& str) {
    const std::vector<char> digits{str.begin(), str.end()};

    std::vector<std::string> digitsStr{};
    std::transform(digits.begin(), digits.end(), std::back_inserter(digitsStr), [](const char& c) {
        return std::string(1, c);
    });

    return strVecToNumVec(digitsStr);
}

////////////////////////////////////////////////////////////////////////////////
// aoc

std::vector<std::optional<size_t>> parseFile() {
    const auto raw = readFile();
    const auto groups = split(raw, "\n\n");

    std::vector<std::optional<size_t>> nums{};
    for (const auto& group : groups) {
        const auto lines = split(group, "\n");
        std::transform(lines.begin(), lines.end(), std::back_inserter(nums), [](const auto& line) {
            return std::stoull(line);
        });
        nums.emplace_back();
    }
    // nums.pop_back();  // remove extra std::nullopt

    return nums;
}

std::vector<size_t> calories() {
    std::vector<size_t> sums{};

    const auto data = parseFile();
    size_t current{};
    for (const auto& line : data) {
        if (!line) {
            sums.emplace_back(current);
            current = 0;
        } else {
            current += *line;
        }
    }

    return sums;
}

size_t part1() {
    const auto cals = calories();
    assert(cals.size());
    return *std::max_element(cals.begin(), cals.end());
}

size_t part2() {
    auto cals = calories();
    std::sort(cals.begin(), cals.end(), std::greater<decltype(cals)::value_type>());  // descending
    assert(cals.size() >= 3);
    return cals[0] + cals[1] + cals[2];
}

int main() {
    const auto p1 = part1();
    std::cout << "part1: " << p1 << '\n';
    assert(p1 == 69626);

    const auto p2 = part2();
    std::cout << "part2: " << p2 << '\n';
    assert(p2 == 206780);
}
