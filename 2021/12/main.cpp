#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
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

std::unordered_map<std::string, std::set<std::string>> parseFile() {
    const auto raw = readFile();
    const auto lines = split(raw, "\n");

    std::vector<std::pair<std::string, std::string>> data{};
    std::transform(lines.begin(), lines.end(), std::back_inserter(data), [](const auto& line) {
        return strVecToStrPair(split(line, "-"));
    });

    std::unordered_map<std::string, std::set<std::string>> edges{};
    for (auto& [node1, node2] : data) {
        const auto insert = [&edges](const auto& n1, const auto& n2) {
            if (const auto it = edges.find(n1); it != edges.end()) {
                auto& [to, from] = *it;
                from.emplace(n2);
            } else {
                edges.emplace(n1, std::set{n2});
            }
        };

        insert(node1, node2);
        insert(node2, node1);
    }

    return edges;
}

size_t part1(bool visitLowerTwice = false) {
    auto edges = parseFile();

    const auto strIsLower = [](const std::string& s) {
        return std::all_of(s.begin(), s.end(), [](const char& c) {
            return std::islower(c);
        });
    };

    std::vector<std::vector<std::string>> options{{"start"}};
    std::vector<std::vector<std::string>> paths{};

    while (options.size()) {
        std::vector<std::vector<std::string>> nextOption{};

        for (const auto& option : options) {
            const auto& current = option.back();

            if (current == "end") {
                paths.emplace_back(option);
            } else {
                for (const auto& node : edges[current]) {
                    if ((node != "start") &&
                        !(strIsLower(node) && std::count(option.begin(), option.end(), node) &&
                          (!visitLowerTwice || std::count(option.begin(), option.end(), "secondvisit")))) {
                        std::vector<std::string> newOption{option.begin(), option.end()};

                        if (visitLowerTwice && strIsLower(node) && std::count(option.begin(), option.end(), node)) {
                            newOption.emplace_back("secondvisit");
                        }

                        newOption.emplace_back(node);
                        nextOption.emplace_back(std::move(newOption));
                    }
                }
            }
        }

        options = nextOption;
    }

    return paths.size();
}

size_t part2() {
    return part1(true);
}

int main() {
    const auto p1 = part1();
    assert(p1 == 5252);
    std::cout << "part1: " << p1 << '\n';

    const auto p2 = part2();
    assert(p2 == 147784);
    std::cout << "part2: " << p2 << '\n';
}
