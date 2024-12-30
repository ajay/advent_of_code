// adventofcode.com/2024/day/6

#include <cassert>
#include <cstddef>
#include <limits>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

enum class Direction {
  Up,
  Right,
  Down,
  Left,
};

struct Grid {
  std::vector<std::string> grid;
  size_t row;
  size_t col;
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

Grid parse(const std::string& path) {
  auto grid = readFile(path);

  size_t row = std::numeric_limits<size_t>::max();
  size_t col = std::numeric_limits<size_t>::max();

  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
    for (size_t c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == '^') {
        assert(row == std::numeric_limits<size_t>::max());
        assert(col == std::numeric_limits<size_t>::max());
        row = r;
        col = c;
      }
    }
  }

  assert(row != std::numeric_limits<size_t>::max());
  assert(col != std::numeric_limits<size_t>::max());

  return Grid{
      .grid = std::move(grid),
      .row = row,
      .col = col,
  };
}

bool exitGrid(std::vector<std::string>& grid,
              size_t row,
              size_t col,
              Direction direction = Direction::Up) {
  const size_t maxOldSpots = (2 * grid.size()) + (2 * grid[0].size()) + 1;
  size_t oldSpots = 0;
  // fmt::println("\n\n{} {}", __func__, maxOldSpots);

  while ((row < grid.size()) && (col < grid[0].size())) {
    if (grid[row][col] == 'X') {
      ++oldSpots;
      // fmt::println("row: {}, col: {}, oldSpots: {}", row, col, oldSpots);
    } else {
      oldSpots = 0;
    }

    if (oldSpots > maxOldSpots) {
      return false;  // loop detected
    }

    grid[row][col] = 'X';
    switch (direction) {
      case Direction::Up:
        if ((row == 0) || (grid[row - 1][col] != '#')) {
          --row;
        } else {
          direction = Direction::Right;
        }
        break;
      case Direction::Right:
        if ((col == (grid[row].size() - 1)) || (grid[row][col + 1] != '#')) {
          ++col;
        } else {
          direction = Direction::Down;
        }
        break;
      case Direction::Down:
        if ((row == (grid.size() - 1)) || (grid[row + 1][col] != '#')) {
          ++row;
        } else {
          direction = Direction::Left;
        }
        break;
      case Direction::Left:
        if ((col == 0) || (grid[row][col - 1] != '#')) {
          --col;
        } else {
          direction = Direction::Up;
        }
        break;
      default:
        assert(false);
        break;
    }
  }

  return true;
}

size_t part1(const std::string& path) {
  auto grid = parse(path);
  assert(exitGrid(grid.grid, grid.row, grid.col));
  return std::accumulate(
      grid.grid.begin(), grid.grid.end(), 0UL, [](const auto& sum, const auto& gridRow) {
        return sum + std::accumulate(
                         gridRow.begin(), gridRow.end(), 0UL,
                         [](const auto& rowSum, const auto& ch) { return rowSum + (ch == 'X'); });
      });
}

size_t part2(const std::string& path) {
  auto grid = parse(path);

  size_t obstacles = 0;
  for (size_t r = 0; r < grid.grid.size(); ++r) {
    for (size_t c = 0; c < grid.grid[r].size(); ++c) {
      auto copy = grid.grid;
      copy[r][c] = '#';
      if (!exitGrid(copy, grid.row, grid.col)) {
        ++obstacles;
      }
    }
  }

  return obstacles;
}

int main() {
  run(1, part1, true, 41UL);
  run(1, part1, false, 4374UL);
  run(2, part2, true, 6UL);
  run(2, part2, false, 1705UL);
}
