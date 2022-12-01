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

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

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

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> parseFile() {
    const auto raw = readFile();
    const auto lines = split(raw, "\n");

    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> data{};

    for (const auto& line : lines) {
        const auto half = split(line, " | ");

        assert(half.size() == 2);
        const auto half0 = split(half[0]);
        const auto half1 = split(half[1]);

        data.emplace_back(std::move(half0), std::move(half1));
    }

    return data;
}

size_t part1() {
    const std::unordered_map<size_t, size_t> nums{
        // {0, 0},
        {1, 2},
        // {2, 0},
        // {3, 0},
        {4, 4},
        // {5, 0},
        // {6, 0},
        {7, 3},
        {8, 7},
        // {9, 0},
    };

    const auto data = parseFile();

    size_t sum = 0;
    for (const auto& [left, right] : data) {
        for (const auto& word : right) {
            const size_t len = word.size();
            for (const auto& [key, value] : nums) {
                if (value == len) {
                    ++sum;
                }
            }
        }
    }

    return sum;
}

size_t part2() {
    // std::unordered_map<size_t, std::vector<std::string>> nums{
    //     {0, {"top", "bottom", "left_top", "left_bottom", "right_top", "right_bottom"}},
    //     {1, {"right_top", "right_bottom"}},
    //     {2, {"top", "middle", "bottom", "left_bottom", "right_top"}},
    //     {3, {"top", "middle", "bottom", "right_top", "right_bottom"}},
    //     {4, {"left_top", "middle", "right_top", "right_bottom"}},
    //     {5, {"top", "middle", "bottom", "left_top", "right_bottom"}},
    //     {6, {"top", "middle", "bottom", "left_top", "left_bottom", "right_bottom"}},
    //     {7, {"top", "right_top", "right_bottom"}},
    //     {8, {"top", "middle", "bottom", "left_top", "left_bottom", "right_top", "right_bottom"}},
    //     {9, {"top", "middle", "bottom", "left_top", "right_top", "right_bottom"}},
    // };

    // std::unordered_map<size_t, std::vector<char>> segmentNums{
    //     {0, {"top", "bottom", "left_top", "left_bottom", "right_top", "right_bottom"}},
    //     {1, {"right_top", "right_bottom"}},
    //     {2, {"top", "middle", "bottom", "left_bottom", "right_top"}},
    //     {3, {"top", "middle", "bottom", "right_top", "right_bottom"}},
    //     {4, {"left_top", "middle", "right_top", "right_bottom"}},
    //     {5, {"top", "middle", "bottom", "left_top", "right_bottom"}},
    //     {6, {"top", "middle", "bottom", "left_top", "left_bottom", "right_bottom"}},
    //     {7, {"top", "right_top", "right_bottom"}},
    //     {8, {"top", "middle", "bottom", "left_top", "left_bottom", "right_top", "right_bottom"}},
    //     {9, {"top", "middle", "bottom", "left_top", "right_top", "right_bottom"}},
    // };

    // const std::unordered_map<size_t, size_t> unique{
    //     // {0, 0},
    //     {1, 2},
    //     // {2, 0},
    //     // {3, 0},
    //     {4, 4},
    //     // {5, 0},
    //     // {6, 0},
    //     {7, 3},
    //     {8, 7},
    //     // {9, 0},
    // };

    // std::unordered_map<std::string, std::vector<char>> segments{
    //     {"top", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"middle", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"bottom", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"left_top", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"left_bottom", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"right_top", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    //     {"right_bottom", {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    // };

    // std::cout << segments << '\n';

    // const auto data = parseFile();

    // for (const auto& [left, right] : data) {
    //     for (const auto& word : left) {
    //         for (const auto& [index, len] : unique) {
    //             if (word.size() == len) {
    //                 const std::vector<char> chars{word.begin(), word.end()};
    //                 const auto& possibleSegments = nums[index];

    //                 for (const auto& seg : possibleSegments) {

    //                     auto& possibleChars = segments[seg];

    //                     const auto newEnd = std::remove_if(possibleChars.begin(), possibleChars.end(), [&chars](const
    //                     auto& c){
    //                         return std::count(chars.begin(), chars.end(), c) == 0;
    //                     });
    //                     possibleChars.erase(newEnd, possibleChars.end());
    //                 }

    //                 // std::cout << chars << '\n';
    //                 // std::cout << index << " : " << possibleSegments << '\n';
    //             }
    //         }
    //     }
    // }

    // std::cout << segments << '\n';
    // for (const auto& [seg, chars] : segments) {
    //     std::cout << "seg: " << chars.size() << '\n';
    // }

    // for (const auto& [left, right] : data) {
    //     for (const auto& word : left) {
    //         std::vector<size_t> possibilities{};
    //         for (const auto& [index, segs] : nums) {

    //             if (word.size() == segs.size()) {
    //                 const std::vector<char> chars{word.begin(), word.end()};

    //                 bool allPresent = true;
    //                 for (const auto& seg : segs) {
    //                     auto& possibleChars = segments[seg];
    //                     // std::cout << "possibleChars: " << possibleChars << '\n';

    //                     for (const auto& c : possibleChars) {
    //                         if (std::count(chars.begin(), chars.end(), c) == 0) {
    //                             allPresent = false;
    //                         }
    //                     }
    //                 }

    //                 if (allPresent) {
    //                     for (const auto& seg : segs) {
    //                         auto& remainingChars = segments[seg];
    //                         // std::cout << index << ' ' << std::setw(20) << seg << " remainingChars: " <<
    //                         remainingChars << '\n'; const auto newEnd = std::remove_if(remainingChars.begin(),
    //                         remainingChars.end(), [&chars](const auto& c){
    //                             return std::count(chars.begin(), chars.end(), c) == 0;
    //                         });
    //                         remainingChars.erase(newEnd, remainingChars.end());
    //                         // std::cout << index << ' ' << std::setw(20) << seg << " remainingChars: " <<
    //                         remainingChars << '\n';
    //                     }

    //                     possibilities.emplace_back(index);
    //                 }
    //             }
    //         }
    //         std::cout << std::setw(15) << std::left << word << ' ';
    //         std::cout << "possibilities: " << possibilities << '\n';
    //     }
    // }

    // std::cout << segments << '\n';
    // for (const auto& [seg, chars] : segments) {
    //     std::cout << "seg: " << chars.size() << '\n';
    // }

    size_t sum = 0;

    const auto data = parseFile();
    for (const auto& [left, right] : data) {
        std::unordered_map<size_t, std::vector<char>> nums{};
        for (size_t i = 0; i <= 0; ++i) {
            nums.emplace(0, std::vector<char>{});
        }

        // numbers with unique length
        const auto findWord = [&left = left](size_t len) {
            std::vector<char> chars{};

            for (const auto& word : left) {
                if (word.length() == len) {
                    chars = {word.begin(), word.end()};
                }
            }

            std::sort(chars.begin(), chars.end());
            return chars;
        };

        nums[1] = findWord(2);
        nums[4] = findWord(4);
        nums[7] = findWord(3);
        nums[8] = findWord(7);

        // numbers with shared length, but known intersections
        const auto findWordIntersection = [&left = left](size_t len, const std::vector<char>& intersect,
                                                         size_t intersectLen) {
            std::vector<char> chars{};

            for (const auto& word : left) {
                if (word.length() == len) {
                    std::vector<char> letters{word.begin(), word.end()};
                    std::sort(letters.begin(), letters.end());

                    std::vector<char> intersection{};

                    std::set_intersection(letters.begin(), letters.end(), intersect.begin(), intersect.end(),
                                          std::back_inserter(intersection));
                    // std::cout << "intersection: " << intersection << '\n';

                    if (intersection.size() == intersectLen) {
                        chars = letters;
                    }
                }
            }

            return chars;
        };

        const auto findWordRemaining = [&left = left](size_t len, const std::vector<std::vector<char>>& nonMatches) {
            std::vector<char> chars{};

            for (const auto& word : left) {
                if (word.length() == len) {
                    std::vector<char> letters{word.begin(), word.end()};
                    std::sort(letters.begin(), letters.end());

                    if (std::count(nonMatches.begin(), nonMatches.end(), letters) == 0) {
                        chars = letters;
                    }
                }
            }

            return chars;
        };

        // 6 does only shares one segment with 1, not both
        nums[6] = findWordIntersection(6, nums[1], 1);

        // 9 has all segments from 4
        nums[9] = findWordIntersection(6, nums[4], 4);

        // 0 is the remaining length 6 number
        nums[0] = findWordRemaining(6, {nums[6], nums[9]});

        // 3 shares both segments with 1
        nums[3] = findWordIntersection(5, nums[1], 2);

        // 5 shares everything in 6
        nums[5] = findWordIntersection(5, nums[6], 5);

        // 2 is the remaining length 5 number
        nums[2] = findWordRemaining(5, {nums[3], nums[5]});

        // std::cout << nums << '\n';

        size_t digits = 0;
        for (const auto& word : right) {
            std::vector<char> letters{word.begin(), word.end()};
            std::sort(letters.begin(), letters.end());
            // std::cout << letters << "    ";

            for (const auto& [index, chars] : nums) {
                if (letters == chars) {
                    digits = (digits * 10) + index;
                    // std::cout << " digits: " << digits << '\n';
                }
            }

            // std::cout << word << ' ' << digits << '\n';
        }
        sum += digits;
        // std::cout << sum << '\n';
    }

    return sum;
}

int main() {
    const auto p1 = part1();
    assert(p1 == 369);
    std::cout << "part1: " << p1 << '\n';

    const auto p2 = part2();
    assert(p2 == 1031553);
    std::cout << "part2: " << p2 << '\n';
}
