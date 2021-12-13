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

std::pair<std::vector<std::pair<size_t, size_t>>, std::vector<std::pair<char, size_t>>> parseFile() {
    const auto raw = readFile();
    const auto lines = split(raw, "\n\n");

    assert(lines.size() == 2);
    const auto points = split(lines[0], "\n");
    const auto folds = split(lines[1], "\n");

    std::vector<std::pair<size_t, size_t>> points_{};
    for (const auto& point : points) {
        const auto xy = split(point, ",");
        points_.emplace_back(strVecToNumPair(xy));
    }

    std::vector<std::pair<char, size_t>> folds_{};
    for (const auto& fold : folds) {
        const auto f = split(fold, "fold along ");
        assert(f.size() == 1);
        const auto dirAndAmount = split(f[0], "=");
        assert(dirAndAmount.size() == 2);
        assert(dirAndAmount[0].size() == 1);
        folds_.emplace_back(dirAndAmount[0][0], std::stoul(dirAndAmount[1]));
    }

    return {points_, folds_};
}

size_t part1(size_t numFolds = 1, bool printGrid = false) {
    const auto [points, folds] = parseFile();

    const size_t maxX = std::max_element(points.begin(), points.end(), [](const auto& p1, const auto& p2) {
                            return p1.first < p2.first;
                        })->first;

    const size_t maxY = std::max_element(points.begin(), points.end(), [](const auto& p1, const auto& p2) {
                            return p1.second < p2.second;
                        })->second;

    std::vector<std::vector<bool>> grid(maxY + 1);
    for (auto& row : grid) {
        row.resize(maxX + 1);
    }

    for (const auto& [x, y] : points) {
        grid[y][x] = true;
    }

    // std::cout << grid << std::endl;
    // std::cout << folds[0] << std::endl;

    for (size_t i = 0; i < (numFolds ?: folds.size()); ++i) {
        const auto& [dir, line] = folds[i];

        if (dir == 'y') {
            if (line >= (grid.size() / 2)) {
                for (size_t y = (line + 1); y < grid.size(); ++y) {
                    const auto newY = (line - (y - line));
                    // std::cout << "y: " << y << " newY: " << newY << '\n';
                    for (size_t x = 0; x < grid[y].size(); ++x) {
                        grid[newY][x] = grid[newY][x] || grid[y][x];
                    }
                }
                grid.resize(line);
            } else {
                assert(false);
            }
        } else if (dir == 'x') {
            if (line >= (grid[0].size() / 2)) {
                for (size_t x = (line + 1); x < grid[0].size(); ++x) {
                    const auto newX = (line - (x - line));
                    // std::cout << "x: " << x << " newX: " << newX << '\n';
                    for (size_t y = 0; y < grid.size(); ++y) {
                        grid[y][newX] = grid[y][newX] || grid[y][x];
                    }
                }

                for (auto& row : grid) {
                    row.resize(line);
                }
            } else {
                assert(false);
            }
        } else {
            assert(false);
        }
    }

    if (printGrid) {
        std::cout << grid << std::endl;
    }

    return std::accumulate(grid.begin(), grid.end(), 0UL, [](const auto& sum, const auto& row) {
        return sum + std::accumulate(row.begin(), row.end(), 0UL, [](const auto& sumRow, const auto& e) {
                   return sumRow + e;
               });
    });
}

void part2() {
    part1(0, true);
}

int main() {
    const auto p1 = part1();
    // assert(p1 == 765);
    std::cout << "part1: " << p1 << '\n';

    part2();
    // assert(p2 == 147784);
    // std::cout << "part2: " << p2 << '\n';
}
