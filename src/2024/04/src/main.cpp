// adventofcode.com/2024/day/4

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

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

size_t check1(const std::vector<std::string>& grid, size_t r, size_t c) {
  size_t count = 0;
  const std::string_view target = "XMAS";

  if (grid[r][c] != target[0]) {
    return count;
  }

  const auto _check = [&grid, &target, &count, &r, &c](auto&& nextIndexFn) {
    for (size_t i = 1; i < target.size(); ++i) {
      const auto& [row, col] = nextIndexFn(r, c, i);

      if (row >= grid.size() || col >= grid[0].size() || grid[row][col] != target[i]) {
        break;
      }
      if (i == (target.size() - 1)) {
        ++count;
      }
    }
  };

  // left
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig, cOrig - i};
  });

  // right
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig, cOrig + i};
  });

  // top
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig - i, cOrig};
  });

  // bottom
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig + i, cOrig};
  });

  // top-left
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig - i, cOrig - i};
  });

  // top-right
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig - i, cOrig + i};
  });

  // bottom-left
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig + i, cOrig - i};
  });

  // bottom-right
  _check([](size_t rOrig, size_t cOrig, size_t i) {
    return std::pair<size_t, size_t>{rOrig + i, cOrig + i};
  });

  return count;
}

bool check2(const std::vector<std::string>& grid, size_t r, size_t c) {
  if (r == 0 || c == 0 || (r == grid.size() - 1) || (c == grid[0].size() - 1)) {
    return false;
  }

  if (grid[r][c] != 'A') {
    return false;
  }

  if (!((grid[r - 1][c - 1] == 'M') || (grid[r - 1][c - 1] == 'S'))) {
    return false;
  }

  if (!((grid[r + 1][c - 1] == 'M') || (grid[r + 1][c - 1] == 'S'))) {
    return false;
  }

  if (grid[r - 1][c - 1] == 'M') {
    if (grid[r + 1][c + 1] != 'S') {
      return false;
    }
  } else {
    if (grid[r + 1][c + 1] != 'M') {
      return false;
    }
  }

  if (grid[r + 1][c - 1] == 'M') {
    if (grid[r - 1][c + 1] != 'S') {
      return false;
    }
  } else {
    if (grid[r - 1][c + 1] != 'M') {
      return false;
    }
  }

  return true;
}

size_t iterate(const std::string& path, bool part2 = false) {
  const auto grid = readFile(path);

  size_t count = 0;
  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[0].size() == grid[r].size());
    for (size_t c = 0; c < grid[r].size(); ++c) {
      count += part2 ? check2(grid, r, c) : check1(grid, r, c);
    }
  }

  return count;
}

size_t part1(const std::string& path) {
  return iterate(path);
}

size_t part2(const std::string& path) {
  return iterate(path, true);
}

int main() {
  run(1, part1, true, 18UL);
  run(1, part1, false, 2514UL);
  run(2, part2, true, 9UL);
  run(2, part2, false, 1888UL);
}
