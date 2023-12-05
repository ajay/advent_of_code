// adventofcode.com/2023/day/3

#include <array>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

std::vector<std::string> readFile(const std::string& path) {
  const auto grid = split(read(path), "\n");
  assert(grid.size());

  const auto expectedColSize = grid.front().size();
  assert(std::all_of(grid.begin(), grid.end(), [&expectedColSize](const auto& row) {
    return row.size() == expectedColSize;
  }));

  return grid;
}

constexpr std::array<std::pair<size_t, size_t>, 8> kDeltas{{
    {0, 1},    // right
    {1, 1},    // bottom-right
    {1, 0},    // bottom
    {1, -1},   // bottom-left
    {0, -1},   // left
    {-1, -1},  // top-left
    {-1, 0},   // top
    {-1, 1},   // top-right
}};

bool checkNeighborPartNumbers(const std::vector<std::string>& grid, size_t row, size_t col) {
  assert(row < grid.size());
  assert(col < grid[0].size());

  const auto checkNeighbor = [&grid](size_t r, size_t c) {
    if ((r >= grid.size()) || (c >= grid[0].size())) {
      return false;
    }

    const auto& ch = grid[r][c];
    return !std::isdigit(ch) && (ch != '.');
  };

  bool found = false;
  for (size_t i = 0; !found && (i < kDeltas.size()); ++i) {
    const auto& [x, y] = kDeltas[i];
    found = checkNeighbor(row + x, col + y);
  }

  return found;
}

size_t gearRatio(const std::vector<std::string>& grid, size_t row, size_t col, char gear = '*') {
  assert(row < grid.size());
  assert(col < grid[0].size());

  if (grid[row][col] != gear) {
    return 0;
  }

  const auto isPartNumber = [&grid](size_t r, size_t c, bool lookLeft = true,
                                    bool lookRight = true) -> std::optional<size_t> {
    if ((r >= grid.size()) || (c >= grid[0].size())) {
      return {};
    }

    if (!std::isdigit(grid[r][c])) {
      return {};
    }

    size_t num = to<size_t>(std::string{grid[r][c]});

    if (lookLeft) {
      size_t i = 1;
      while ((i <= c) && std::isdigit(grid[r][c - i])) {
        size_t add = to<size_t>(std::string{grid[r][c - i]});
        for (size_t x = 0; x < i; ++x) {
          add *= 10;
        }

        num = num + add;
        ++i;
      }
    }

    if (lookRight) {
      size_t i = 1;
      while ((c + i) < grid.size() && std::isdigit(grid[r][c + i])) {
        num = (10 * num) + to<size_t>(std::string{grid[r][c + i]});
        ++i;
      }
    }

    return num;
  };

  std::vector<size_t> partNumbers{};

  if (const auto pn = isPartNumber(row, col - 1, true, false); pn) {  // left
    partNumbers.emplace_back(*pn);
  }
  if (const auto pn = isPartNumber(row, col + 1, false, true); pn) {  // right
    partNumbers.emplace_back(*pn);
  }

  if (auto pn = isPartNumber(row - 1, col); pn) {  // top
    partNumbers.emplace_back(*pn);
  } else {
    if (pn = isPartNumber(row - 1, col - 1, true, false); pn) {  // top-left
      partNumbers.emplace_back(*pn);
    }
    if (pn = isPartNumber(row - 1, col + 1, false, true); pn) {  // top-right
      partNumbers.emplace_back(*pn);
    }
  }

  if (auto pn = isPartNumber(row + 1, col); pn) {  // bottom
    partNumbers.emplace_back(*pn);
  } else {
    if (pn = isPartNumber(row + 1, col - 1, true, false); pn) {  // bottom-left
      partNumbers.emplace_back(*pn);
    }
    if (pn = isPartNumber(row + 1, col + 1, false, true); pn) {  // bottom-right
      partNumbers.emplace_back(*pn);
    }
  }

  return (partNumbers.size() == 2) ? (partNumbers[0] * partNumbers[1]) : 0;
}

size_t part1(const std::string& path) {
  size_t sum = 0;
  struct {
    size_t num = 0;
    bool isNum = false;
    bool symbol = false;
    uint8_t _reserved[6];
  } state;

  const auto grid = readFile(path);

  const auto check = [&state, &grid](size_t r, size_t c) {
    if (!state.symbol) {
      state.symbol = checkNeighborPartNumbers(grid, r, c);
    }
  };

  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      const auto& ch = grid[r][c];

      if (!state.isNum && std::isdigit(ch)) {
        state = {
            .num = to<size_t>(std::string{ch}),
            .isNum = true,
        };
        check(r, c);
      } else if (state.isNum) {
        if ((c == 0) || !(std::isdigit(ch))) {
          if (state.symbol) {
            sum += state.num;
          }
          state = {};
        } else {
          state.num = (state.num * 10) + to<size_t>(std::string{ch});
          check(r, c);
        }
      }
    }
  }

  return sum;
}

size_t part2(const std::string& path) {
  const auto grid = readFile(path);

  size_t sum = 0;
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      sum += gearRatio(grid, r, c);
    }
  }

  return sum;
}

}  // namespace

int main() {
  run(1, part1, true, 4361UL);
  run(1, part1, false, 507214UL);
  run(2, part2, true, 467835UL);
  run(2, part2, false, 72553319UL);
}
