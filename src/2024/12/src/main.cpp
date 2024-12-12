// adventofcode.com/2024/day/12

#include <algorithm>
#include <array>
#include <cctype>
#include <list>
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

void traverse(char ch,
              size_t r,
              size_t c,
              const std::vector<std::string>& grid,
              std::vector<std::vector<bool>>& seen,
              std::vector<std::pair<size_t, size_t>>& region) {
  if (r >= grid.size() || c >= grid[r].size() || seen[r][c] || grid[r][c] != ch) {
    return;
  }

  seen[r][c] = true;
  region.emplace_back(r, c);

  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  for (const auto& [dirR, dirC] : directions) {
    const auto newR = static_cast<size_t>(static_cast<ssize_t>(r) + dirR);
    const auto newC = static_cast<size_t>(static_cast<ssize_t>(c) + dirC);

    traverse(ch, newR, newC, grid, seen, region);
  }
}

std::vector<std::vector<std::pair<size_t, size_t>>> findRegions(
    const std::vector<std::string>& grid) {
  std::vector<std::vector<std::pair<size_t, size_t>>> regions;
  std::vector<std::vector<bool>> seen(grid.size(), std::vector<bool>(grid[0].size(), false));
  // fmt::println("seen: {}", seen);

  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
    for (size_t c = 0; c < grid[r].size(); ++c) {
      std::vector<std::pair<size_t, size_t>> region{};
      traverse(grid[r][c], r, c, grid, seen, region);
      if (!region.empty()) {
        regions.emplace_back(std::move(region));
      }
    }
  }

  return regions;
}

size_t perimeter(const std::vector<std::string>& grid,
                 const std::vector<std::pair<size_t, size_t>>& region,
                 bool bulk) {
  size_t perimeter = 0;
  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  for (const auto& [r, c] : region) {
    for (const auto& [dirR, dirC] : directions) {
      const auto newR = static_cast<size_t>(static_cast<ssize_t>(r) + dirR);
      const auto newC = static_cast<size_t>(static_cast<ssize_t>(c) + dirC);

      if (newR >= grid.size() || newC >= grid[newR].size() ||
          (newR < grid.size() && newC < grid[newR].size() && grid[r][c] != grid[newR][newC])) {
        ++perimeter;

        if (bulk) {
          const auto check = [&grid, &r, &c](const auto& bulkR, const auto& bulkC,
                                             bool invert = false) -> bool {
            if (bulkR >= grid.size() || bulkC >= grid[bulkR].size()) {
              return invert;
            }

            return invert ? grid[bulkR][bulkC] != grid[r][c] : grid[bulkR][bulkC] == grid[r][c];
          };

          if ((dirR == 0 && dirC == 1 && check(r + 1, c) && check(r + 1, c + 1, true)) ||
              (dirR == 0 && dirC == -1 && check(r + 1, c) && check(r + 1, c - 1, true)) ||
              (dirR == 1 && dirC == 0 && check(r, c + 1) && check(r + 1, c + 1, true)) ||
              (dirR == -1 && dirC == 0 && check(r, c + 1) && check(r - 1, c + 1, true))) {
            --perimeter;
          }
        }
      }
    }
  }

  return perimeter;
}

size_t cost(const std::string& path, bool bulk = false) {
  const auto grid = readFile(path);
  const auto regions = findRegions(grid);
  // fmt::println("grid:    ({}x{}) \n\t {}", grid.size(), grid[0].size(), grid);
  // fmt::println("regions: ({})    \n\t {}", regions.size(), regions);

  return std::accumulate(regions.begin(), regions.end(), 0UL,
                         [&grid, &bulk](const auto& sum, const auto& region) {
                           const auto per = perimeter(grid, region, bulk);
                           const auto price = per * region.size();

                           // fmt::println("region:  ({} - {})  perimeter: {}  price: {}\n\t {}",
                           // region.size(),
                           //              grid[region[0].first][region[0].second], per, price,
                           //              region);

                           return sum + price;
                         });
}

size_t part1(const std::string& path) {
  return cost(path);
}

size_t part2(const std::string& path) {
  return cost(path, true);
}

int main() {
  run(1, part1, true, 140UL, "data/example.txt");
  run(1, part1, true, 772UL, "data/example2.txt");
  run(1, part1, true, 1930UL, "data/example3.txt");
  run(1, part1, false, 1477924UL);
  run(2, part2, true, 80UL, "data/example.txt");
  run(2, part2, true, 436UL, "data/example2.txt");
  run(2, part2, true, 1206UL, "data/example3.txt");
  run(2, part2, false, 841934UL);
}
