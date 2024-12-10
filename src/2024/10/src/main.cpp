// adventofcode.com/2024/day/10

#include <algorithm>
#include <array>
#include <cctype>
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

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::vector<std::vector<ssize_t>> parse(const std::string& path) {
  std::vector<std::vector<ssize_t>> grid;
  for (auto&& line : readFile(path)) {
    std::vector<ssize_t> row;
    std::transform(line.begin(), line.end(), std::back_inserter(row),
                   [](const auto& ch) { return to<ssize_t>(std::string{ch}); });
    grid.emplace_back(std::move(row));
  }

  return grid;
}

size_t score(const std::vector<std::vector<ssize_t>>& grid,
             std::vector<std::vector<size_t>>& seen,
             ssize_t r,
             ssize_t c,
             bool part2) {
  const auto r_ = static_cast<size_t>(r);
  const auto c_ = static_cast<size_t>(c);

  size_t result = 0;
  if (grid[r_][c_] == 9) {
    if (!part2) {
      if (seen[r_][c_] == 0) {
        result = 1;
      }
      ++seen[r_][c_];
      return result;
    } else {  // part 2
      return 1;
    }
  }

  ssize_t height = grid[r_][c_];

  const auto valid = [&grid](const auto& row, const auto& col) {
    return ((row >= 0) && (row < static_cast<decltype(row)>(grid.size())) && (col >= 0) &&
            (col < static_cast<decltype(col)>(grid[static_cast<size_t>(row)].size())));
  };

  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (const auto& [dirR, dirC] : directions) {
    if (valid(r + dirR, c + dirC) &&
        grid[static_cast<size_t>(r + dirR)][static_cast<size_t>(c + dirC)] == height + 1) {
      result += score(grid, seen, r + dirR, c + dirC, part2);
    }
  }

  return result;
}

size_t solve(const std::vector<std::vector<ssize_t>>& grid, bool part2 = false) {
  size_t result = 0;
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == 0) {
        std::vector<std::vector<size_t>> seen(grid.size(), std::vector<size_t>(grid[r].size(), 0));
        result += score(grid, seen, static_cast<ssize_t>(r), static_cast<ssize_t>(c), part2);
      }
    }
  }

  return result;
}

size_t part1(const std::string& path) {
  return solve(parse(path));
}

size_t part2(const std::string& path) {
  return solve(parse(path), true);
}

int main() {
  run(1, part1, true, 1UL, "data/example.txt");
  run(1, part1, true, 36UL, "data/example2.txt");
  run(1, part1, false, 746UL);
  run(2, part2, true, 81UL, "data/example2.txt");
  run(2, part2, false, 1541UL);
}
