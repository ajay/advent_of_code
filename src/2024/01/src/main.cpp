// adventofcode.com/2024/day/1

#include <algorithm>
#include <array>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::pair<std::vector<size_t>, std::vector<size_t>> parse(const std::string& path) {
  std::vector<size_t> left;
  std::vector<size_t> right;

  auto data = readFile(path);
  for (auto&& line : data) {
    const auto& [lhs, rhs] = splitToPair<size_t, size_t>(std::move(line));
    // fmt::print("lhs: {}, rhs: {}\n", lhs, rhs);
    left.emplace_back(lhs);
    right.emplace_back(rhs);
  }

  assert(left.size() == right.size());
  return {left, right};
}

size_t part1(const std::string& path) {
  auto [left, right] = parse(path);
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());
  // fmt::print("{}\n", left);
  // fmt::print("{}\n", right);

  size_t distance = 0;
  for (size_t i = 0; i < left.size(); ++i) {
    distance += static_cast<decltype(distance)>(
        std::abs(static_cast<ssize_t>(right[i]) - static_cast<ssize_t>(left[i])));
  }

  return distance;
}

size_t part2(const std::string& path) {
  auto [left, right] = parse(path);

  std::unordered_map<size_t, size_t> counts;
  for (const auto& num : right) {
    if (counts.count(num)) {
      counts[num] += 1;
    } else {
      counts[num] = 1;
    }
  }

  return std::accumulate(left.begin(), left.end(), 0UL,
                         [&counts](const auto& similarity, const auto& num) {
                           return similarity + (counts.count(num) ? (num * counts[num]) : 0UL);
                         });
}

int main() {
  run(1, part1, true, 11UL);
  run(1, part1, false, 1530215UL);
  run(2, part2, true, 31UL);
  run(2, part2, false, 26800609UL);
}
