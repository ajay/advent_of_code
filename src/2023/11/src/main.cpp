// adventofcode.com/2023/day/11

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>

#if defined __has_include && __has_include("fmt/base.h")
#include <fmt/base.h>
#endif
#include <fmt/core.h>  // IWYU pragma: keep
#include <fmt/ranges.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

void print(const std::vector<std::vector<char>>& grid, bool log = true) {
  if (log) {
    fmt::print("   ");
    for (size_t c = 0; c < grid[0].size(); ++c) {
      fmt::print("{:2}", c);
    }
    fmt::print("\n");

    for (size_t r = 0; r < grid.size(); ++r) {
      fmt::print("{:2} [{}]\n", r, fmt::join(grid[r], " "));
    }
    fmt::print("\n");
  }
}

std::vector<std::vector<char>> readFile(const std::string& path) {
  auto lines = split(read(path), "\n");

  std::vector<std::vector<char>> grid{};
  for (auto&& line : lines) {
    grid.emplace_back(to<std::vector<char>>(std::move(line)));
  }

  assert(grid.size());
  assert(grid[0].size());
  assert(std::all_of(grid.begin(), grid.end(),
                     [&grid](const auto& row) { return row.size() == grid[0].size(); }));

  return grid;
}

struct GridExpanded {
  std::vector<std::vector<char>> grid;
  std::vector<size_t> expandedRows = {};
  std::vector<size_t> expandedCols = {};
};

GridExpanded expandGrid(std::vector<std::vector<char>>&& gridInner,
                        size_t expansion = 1,
                        char empty = '.',
                        bool log = false) {
  GridExpanded gridExpanded = {.grid = std::move(gridInner)};
  auto& grid = gridExpanded.grid;
  print(grid, log);
  const auto noGalaxiesRow = [&empty](const auto& row) {
    return std::all_of(row.begin(), row.end(), [&empty](const auto& ch) { return ch == empty; });
  };

  for (size_t r = 0; r < grid.size(); ++r) {
    if (noGalaxiesRow(grid[r])) {
      gridExpanded.expandedRows.emplace_back(r);
      for (size_t e = 0; e < expansion; ++e) {
        const auto it = std::next(grid.begin(), static_cast<ssize_t>(r++));
        grid.insert(it, *it);
      }
    }
  }
  print(grid, log);

  for (size_t c = 0; c < grid[0].size(); ++c) {
    bool noGalaxiesCol = true;
    for (size_t r = 0; noGalaxiesCol && (r < grid.size()); ++r) {
      noGalaxiesCol = noGalaxiesCol && (grid[r][c] == empty);
    }
    if (noGalaxiesCol) {
      gridExpanded.expandedCols.emplace_back(c);
      for (size_t e = 0; e < expansion; ++e) {
        for (size_t r = 0; r < grid.size(); ++r) {
          const auto it = std::next(grid[r].begin(), static_cast<ssize_t>(c));
          grid[r].insert(it, *it);
        }
        ++c;
      }
    }
  }
  print(grid, log);

  return gridExpanded;
}

size_t sumOfDistances(const GridExpanded& gridExpanded,
                      size_t extraDistance = 0,
                      char galaxy = '#') {
  const auto& grid = gridExpanded.grid;
  std::vector<std::pair<size_t, size_t>> galaxies{};
  size_t sum = 0;
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == galaxy) {
        galaxies.emplace_back(c, r);
      }
    }
  }
  // fmt::print("Galaxies: {}\n", galaxies);

  for (size_t i = 0; i < galaxies.size() - 1; ++i) {
    for (size_t j = i + 1; j < galaxies.size(); ++j) {
      const auto& [x1, y1] = galaxies[i];
      const auto& [x2, y2] = galaxies[j];
      const auto diffX = (x1 > x2) ? (x1 - x2) : (x2 - x1);
      const auto diffY = (y1 > y2) ? (y1 - y2) : (y2 - y1);
      const auto diff = diffX + diffY;

      // fmt::print("{} - {} | {} <-> {}          {}\n", i, j, galaxies[i], galaxies[j], diff);
      sum += diff;

      const auto xMax = std::max(x1, x2);
      const auto xMin = std::min(x1, x2);
      const auto yMax = std::max(y1, y2);
      const auto yMin = std::min(y1, y2);

      for (const auto& eCol : gridExpanded.expandedCols) {
        // fmt::print("col: {}\n", eCol);
        if ((xMax > eCol) && (eCol > xMin)) {
          // fmt::print("X Adding extra distance '{}'     p1: ({}, {})   p2: ({}, {})  eCol: {}\n",
          // extraDistance, x1, y1, x2, y2, eCol);
          sum += extraDistance;
        }
      }
      for (const auto& eRow : gridExpanded.expandedRows) {
        // fmt::print("row: {}, yMax: {}, yMin: {}\n", eRow, yMax, yMin);
        if ((yMax > eRow) && (eRow > yMin)) {
          // fmt::print("Y Adding extra distance '{}'     p1: ({}, {})   p2: ({}, {})  eRow: {}\n",
          // extraDistance, x1, y1, x2, y2, eRow);
          sum += extraDistance;
        }
      }
    }
  }

  return sum;
}

size_t part1(const std::string& path) {
  return sumOfDistances(expandGrid(readFile(path), 0), 1);
}

size_t part2(const std::string& path) {
  return sumOfDistances(expandGrid(readFile(path), 0), 1000000 - 1);
}

}  // namespace

int main() {
  (void)part1;
  run(1, part1, true, 374UL);
  run(1, part1, false, 9623138UL);
  // run(2, part2, true, 8410UL); with expansion = (100 -1)
  run(2, part2, false, 726820169514UL);
}
