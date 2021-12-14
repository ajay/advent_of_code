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

std::pair<std::string, std::unordered_map<std::string, char>> parseFile() {
    const auto raw = readFile();
    auto data = split(raw, "\n\n");
    // data[0].reserve(10'000'000'000);

    assert(data.size() == 2);

    const auto lines = split(data[1], "\n");

    std::unordered_map<std::string, char> rules{};
    for (const auto& line : lines) {
        auto keyValue = split(line, " -> ");
        assert(keyValue.size() == 2);
        assert(keyValue[1].size() == 1);
        rules.emplace(std::move(keyValue[0]), keyValue[1][0]);
    }

    return {std::move(data[0]), std::move(rules)};
}

size_t part1(size_t steps = 10) {
    const auto [polymer, rules] = parseFile();

    // const auto step = [&polymer=polymer, &rules=rules]() {
    //     auto pairBegin = polymer.begin();
    //     auto pairEnd = std::next(pairBegin, 2);

    //     while (pairEnd <= polymer.end()) {
    //         const std::string key{pairBegin, pairEnd};

    //         if (const auto it = rules.find(key); it != rules.end()) {
    //             const auto& [pair, ch] = *it;
    //             polymer.insert(std::next(pairBegin), ch);
    //         }

    //         pairBegin = pairEnd;
    //         pairEnd = std::next(pairBegin, 2);
    //     }
    // };

    // for (size_t i = 0; i < steps; ++i) {
    //     step();
    //     // std::cout << "step " << i+1 << ": " << polymer << std::endl;
    //     std::cout << "step " << (i+1) << ": " << polymer.size() << std::endl;
    // }

    // std::unordered_map<char, size_t> histogram{};
    // for (const auto& ch : polymer) {
    //     if (const auto it = histogram.find(ch); it != histogram.end()) {
    //         auto& [key, count] = *it;
    //         ++count;
    //     } else {
    //         histogram.emplace(ch, 1);
    //     }
    // }

    std::map<std::string, size_t> pairs{};

    auto pairBegin = polymer.begin();
    while (std::next(pairBegin, 2) <= polymer.end()) {
        const std::string key{pairBegin, std::next(pairBegin, 2)};

        if (const auto it = pairs.find(key); it != pairs.end()) {
            auto& [pair, count] = *it;
            ++count;
        } else {
            pairs.emplace(key, 1);
        }

        ++pairBegin;
    }

    const auto step = [&pairs, &rules = rules]() {
        std::map<std::string, size_t> newPairs{};

        for (const auto& [pair, countPair] : pairs) {
            if (const auto it = rules.find(pair); it != rules.end()) {
                const auto& [pair_, ch] = *it;

                const auto insert = [&newPairs, &countPair = countPair](const std::string& s) {
                    if (const auto newPairIt = newPairs.find(s); newPairIt != newPairs.end()) {
                        auto& [newPair, count] = *newPairIt;
                        count += countPair;
                    } else {
                        newPairs.emplace(s, countPair);
                    }
                };

                assert(pair.size() == 2);
                insert({pair[0], ch});
                insert({ch, pair[1]});
            }
        }

        pairs = newPairs;
    };

    for (size_t i = 0; i < steps; ++i) {
        step();
        // std::cout << "step " << i+1 << ": " << pairs;
        // const auto sum = std::accumulate(pairs.begin(), pairs.end(), 0UL, [](const auto& sum_, const auto& pair) {
        //     return sum_ + pair.second;
        // });
        // std::cout << "sum: " << sum << std::endl << std::endl;

        // std::cout << "step " << (i+1) << ": " << polymer.size() << std::endl;
    }

    std::unordered_map<char, size_t> histogram{};
    const auto histogramInsert = [&histogram](char c, size_t count = 1) {
        if (const auto it = histogram.find(c); it != histogram.end()) {
            auto& [key, count_] = *it;
            count_ += count;
        } else {
            histogram.emplace(c, count);
        }
    };

    for (const auto& pair : pairs) {
        const auto& [str, count] = pair;
        assert(str.size() == 2);

        histogramInsert(str[1], count);
    }

    const auto& [lastStr, lastCount] = *std::prev(pairs.end());
    histogramInsert(lastStr[0], lastCount);

    // std::cout << histogram << std::endl;

    return std::max_element(histogram.begin(), histogram.end(),
                            [](const auto& p1, const auto& p2) {
                                return p1.second < p2.second;
                            })
               ->second -
           std::min_element(histogram.begin(), histogram.end(), [](const auto& p1, const auto& p2) {
               return p1.second < p2.second;
           })->second;
}

size_t part2() {
    return part1(40);
}

int main() {
    const auto p1 = part1();
    std::cerr << "part1: " << p1 << '\n';
    assert(p1 == 2068);

    const auto p2 = part2();
    assert(p2 == 2158894777814);
    std::cout << "part2: " << p2 << '\n';
}
