// adventofcode.com/2023/day/10

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

namespace {

struct Point {
  size_t x;
  size_t y;
};

bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct PointStep {
  Point point;
  size_t steps;
};

enum class Pipe : char {
  Vertical = '|',
  Horizontal = '-',
  NorthEast = 'L',
  NorthWest = 'J',
  SouthEast = 'F',
  SouthWest = '7',
  Ground = '.',
  Start = 'S',
  None = '\0',
};

bool isValidPipe(char c) {
  switch (c) {
    case static_cast<char>(Pipe::Vertical):
    case static_cast<char>(Pipe::Horizontal):
    case static_cast<char>(Pipe::NorthEast):
    case static_cast<char>(Pipe::NorthWest):
    case static_cast<char>(Pipe::SouthEast):
    case static_cast<char>(Pipe::SouthWest):
    case static_cast<char>(Pipe::Ground):
    case static_cast<char>(Pipe::Start):
      return true;
    case static_cast<char>(Pipe::None):
    default:
      return false;
  }
}

bool checkSpot(const std::vector<std::string>& grid, const Point& point, Pipe pipe) {
  if ((point.y >= grid.size()) || (point.x >= grid[0].size())) {
    return false;
  }

  if (static_cast<Pipe>(grid[point.y][point.x]) == pipe) {
    return true;
  }

  return false;
}

std::vector<std::string> readFile(const std::string& path) {
  const auto grid = split(read(path), "\n");

  assert(std::all_of(grid.begin(), grid.end(), [](const auto& row) {
    return std::all_of(row.begin(), row.end(), [](const auto& ch) { return isValidPipe(ch); });
  }));

  assert(grid.size());
  assert(std::all_of(grid.begin(), grid.end(),
                     [&grid](const auto& row) { return row.size() == grid[0].size(); }));

  return grid;
}

Point find(const std::vector<std::string>& grid, Pipe pipe) {
  for (size_t y = 0; y < grid.size(); ++y) {
    for (size_t x = 0; x < grid[y].size(); ++x) {
      if (grid[y][x] == static_cast<char>(pipe)) {
        return {.x = x, .y = y};
      }
    }
  }

  assert(false);
  return {};
}

std::vector<PointStep> searchLoop(const std::vector<std::string>& grid,
                                  size_t startX,
                                  size_t startY) {
  std::vector<PointStep> loop{};
  std::queue<PointStep> queue{{PointStep{{.x = startX, .y = startY}, 0}}};

  while (!queue.empty()) {
    const auto loopIt = std::find_if(loop.begin(), loop.end(), [&queue](const auto& ps) {
      return ps.point == queue.front().point;
    });

    if (loopIt != loop.end()) {
      loopIt->steps = std::min(loopIt->steps, queue.front().steps);
      queue.pop();
    } else {
      loop.emplace_back(std::move(queue.front()));
      queue.pop();
      const auto& point = loop.back().point;
      const auto& [x, y] = point;
      const auto& steps = loop.back().steps;

      const auto checkOrigin = [&grid](const Point& p, const std::vector<Pipe>& pipes,
                                       const std::function<void(const Point&)>& fn = nullptr) {
        for (const auto& pipe : pipes) {
          if (checkSpot(grid, p, pipe)) {
            if (fn) {
              fn(p);
            }
            return true;
          }
        }
        return false;
      };

      const auto checkNeighbor = [&queue, &steps, &checkOrigin](const Point& p,
                                                                const std::vector<Pipe>& pipes) {
        checkOrigin(p, pipes,
                    [&queue, &steps](const auto& pt) { queue.emplace(PointStep{pt, steps + 1}); });
      };

      if (checkOrigin(point, {Pipe::Start, Pipe::Vertical, Pipe::NorthEast, Pipe::NorthWest})) {
        checkNeighbor({.x = x, .y = y - 1}, {Pipe::Vertical, Pipe::SouthEast, Pipe::SouthWest});
      }

      if (checkOrigin(point, {Pipe::Start, Pipe::Vertical, Pipe::SouthEast, Pipe::SouthWest})) {
        checkNeighbor({.x = x, .y = y + 1}, {Pipe::Vertical, Pipe::NorthEast, Pipe::NorthWest});
      }

      if (checkOrigin(point, {Pipe::Start, Pipe::Horizontal, Pipe::NorthWest, Pipe::SouthWest})) {
        checkNeighbor({.x = x - 1, .y = y}, {Pipe::Horizontal, Pipe::NorthEast, Pipe::SouthEast});
      }

      if (checkOrigin(point, {Pipe::Start, Pipe::Horizontal, Pipe::NorthEast, Pipe::SouthEast})) {
        checkNeighbor({.x = x + 1, .y = y}, {Pipe::Horizontal, Pipe::NorthWest, Pipe::SouthWest});
      }
    }
  }

  return loop;
}

size_t part1(const std::string& path) {
  const auto grid = readFile(path);

  const auto [x, y] = find(grid, Pipe::Start);
  // fmt::print("Start x: {}, y: {}\n", x, y);

  const auto loop = searchLoop(grid, x, y);
  // for (const auto& ps : loop) {
  //   fmt::print("PointStep: [.x = {}, .y = {}], .steps = {}\n", ps.point.x, ps.point.y, ps.steps);
  // }

  return std::max_element(loop.begin(), loop.end(),
                          [](const auto& lhs, const auto& rhs) { return lhs.steps < rhs.steps; })
      ->steps;
}

size_t part2(const std::string& path) {
  const auto grid = readFile(path);

  const auto [sX, sY] = find(grid, Pipe::Start);
  const auto loop = searchLoop(grid, sX, sY);

  size_t enclosed = 0;
  for (size_t y = 0; y < grid.size(); ++y) {
    bool inside = false;
    Pipe prev = Pipe::None;
    for (size_t x = 0; x < grid[y].size(); ++x) {
      const auto loopIt = std::find_if(loop.begin(), loop.end(), [&x, &y](const auto& ps) {
        return ps.point == Point{.x = x, .y = y};
      });

      if (loopIt != loop.end()) {
        const auto pipe = static_cast<Pipe>(grid[y][x]);
        if ((pipe == Pipe::NorthEast) || (pipe == Pipe::SouthEast)) {
          prev = pipe;
        } else if ((pipe == Pipe::Vertical) || (pipe == Pipe::NorthWest) ||
                   (pipe == Pipe::SouthWest)) {
          if (((prev == Pipe::None) && (pipe == Pipe::Vertical)) ||
              ((prev == Pipe::SouthEast) && (pipe == Pipe::NorthWest)) ||
              ((prev == Pipe::NorthEast) && (pipe == Pipe::SouthWest))) {
            inside = !inside;
          }
          prev = Pipe::None;
          // fmt::print("Flipping inside      at ({}, {}): '{}' [{}]\n", x, y, grid[y][x], inside);
        }
      } else if (inside) {
        ++enclosed;
        // fmt::print("  Adding to enclosed at ({}, {}): '{}' [{}]\n", x, y, grid[y][x], enclosed);
      }
    }
  }

  return enclosed;
}

}  // namespace

int main() {
  run(1, part1, true, 4UL);
  run(1, part1, true, 8UL, "data/example2.txt");
  run(1, part1, false, 6867UL);
  run(2, part2, true, 4UL, "data/example3.txt");
  run(2, part2, true, 8UL, "data/example4.txt");
  run(2, part2, true, 10UL, "data/example5.txt");
  run(2, part2, false, 595UL);
}
