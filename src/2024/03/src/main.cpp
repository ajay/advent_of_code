// adventofcode.com/2024/day/3

#include <algorithm>
#include <array>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::string readFile(const std::string& path) {
  return read(path);
}

size_t parse(const std::string& path, bool dodont = false) {
  auto data = readFile(path);

  std::smatch match;
  std::regex regex{R"(mul\(([0-9]+),([0-9]+)\)|do\(\)|don\'t\(\))"};

  size_t result = 0;
  bool enabled = true;

  std::string::const_iterator start(data.cbegin());
  while (std::regex_search(start, data.cend(), match, regex)) {
    // fmt::println("match: '{}' '{}' '{}' ({})", std::string{match[0]}, std::string{match[1]},
    //              std::string{match[2]}, match.size());

    if (match.length(1) == 0 && match.length(2) == 0) {
      if (dodont) {
        if (match[0] == "do()") {
          enabled = true;
        } else if (match[0] == "don't()") {
          enabled = false;
        } else {
          assert(false);
        }
      }
    } else {
      assert(match.size() == 3);
      const auto num1 = to<size_t>(std::string{match[1]});
      const auto num2 = to<size_t>(std::string{match[2]});
      // fmt::println("match: {}, num1: {}, num2: {}", std::string{match[0]}, num1, num2);

      if (enabled) {
        result += (num1 * num2);
      }
    }
    start = match.suffix().first;
  }

  return result;
}

size_t part1(const std::string& path) {
  return parse(path);
}

size_t part2(const std::string& path) {
  return parse(path, true);
}

int main() {
  run(1, part1, true, 161UL);
  run(1, part1, false, 178538786UL);
  run(2, part2, true, 48UL, "data/example2.txt");
  run(2, part2, false, 102467299UL);
}
