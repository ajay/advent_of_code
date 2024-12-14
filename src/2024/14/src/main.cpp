// adventofcode.com/2024/day/14

#include <algorithm>
#include <array>
#include <cctype>
#include <list>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

struct Robot {
  struct Point {
    ssize_t x;
    ssize_t y;
  };

  Point position;
  Point velocity;
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::vector<Robot> parse(const std::string& path) {
  auto lines = readFile(path);
  std::vector<Robot> robots{};

  for (auto&& robotStr : lines) {
    auto parts = split(std::move(robotStr), "=, ");
    assert(parts.size() == 6);
    assert(parts[0] == "p");
    assert(parts[3] == "v");

    robots.emplace_back(Robot{
        .position =
            {
                .x = to<ssize_t>(std::move(parts[1])),
                .y = to<ssize_t>(std::move(parts[2])),
            },
        .velocity =
            {
                .x = to<ssize_t>(std::move(parts[4])),
                .y = to<ssize_t>(std::move(parts[5])),
            },
    });
  }

  return robots;
}

void advance(std::vector<Robot>& robots, ssize_t sizeX, ssize_t sizeY) {
  for (auto& robot : robots) {
    robot.position.x += robot.velocity.x;
    robot.position.y += robot.velocity.y;

    robot.position.x %= sizeX;
    robot.position.y %= sizeY;

    if (robot.position.x < 0) {
      robot.position.x += sizeX;
    }

    if (robot.position.y < 0) {
      robot.position.y += sizeY;
    }
  }
}

size_t simulate(const std::string& path, ssize_t sizeX, ssize_t sizeY, size_t seconds = 100) {
  auto robots = parse(path);

  for (size_t i = 0; i < seconds; ++i) {
    advance(robots, sizeX, sizeY);
  }

  assert(sizeX % 2 == 1);
  assert(sizeY % 2 == 1);
  const ssize_t midX = sizeX / 2;
  const ssize_t midY = sizeY / 2;

  std::array<size_t, 4> quadrants = {};
  for (const auto& robot : robots) {
    const auto& x = robot.position.x;
    const auto& y = robot.position.y;

    // low = inclusive, high = exclusive
    const auto inRange = [](ssize_t low, ssize_t val, ssize_t high) {
      return (low <= val) && (val < high);
    };

    // fmt::println("x: {}, y: {}", x, y);
    if (inRange(0, x, midX) && inRange(0, y, midY)) {
      ++quadrants[0];
    } else if (inRange(0, x, midX) && inRange((midY + 1), y, sizeY)) {
      ++quadrants[1];
    } else if (inRange((midX + 1), x, sizeX) && inRange((midY + 1), y, sizeY)) {
      ++quadrants[2];
    } else if (inRange((midX + 1), x, sizeX) && inRange(0, y, midY)) {
      ++quadrants[3];
    } else {
      // assert(false);
    }
  }

  // fmt::println("quadrants: {}", quadrants);
  return std::accumulate(quadrants.begin(), quadrants.end(), 1UL, std::multiplies<size_t>());
}

size_t tree(const std::string& path, ssize_t sizeX, ssize_t sizeY) {
  auto robots = parse(path);
  size_t count = 0;
  std::vector<std::vector<bool>> grid{};
  size_t num = 0;
  do {
    ++count;
    num = 0;
    grid = std::vector<std::vector<bool>>(static_cast<size_t>(sizeY),
                                          std::vector<bool>(static_cast<size_t>(sizeX), false));

    advance(robots, sizeX, sizeY);
    for (const auto& robot : robots) {
      const auto x = static_cast<size_t>(robot.position.x);
      const auto y = static_cast<size_t>(robot.position.y);
      grid[y][x] = true;
    }

    num = std::accumulate(grid.begin(), grid.end(), 0UL, [](const auto& sum, const auto& row) {
      return sum +
             std::accumulate(row.begin(), row.end(), 0UL,
                             [](const auto& rowSum, const auto& val) { return rowSum + val; });
    });

    // fmt::println("count: {}, num: {}, robots.size: {}, grid: {}\n\n", count, num, robots.size(),
    // grid);

  } while (num != robots.size());

  return count;
}

size_t part1(const std::string& path) {
  const bool example = (path == "data/example.txt");
  return simulate(path, example ? 11 : 101, example ? 7 : 103);
}

size_t part2(const std::string& path) {
  const bool example = (path == "data/example.txt");
  return tree(path, example ? 11 : 101, example ? 7 : 103);
}

int main() {
  run(1, part1, true, 12UL);
  run(1, part1, false, 218619324UL);
  // run(2, part2, true, 1UL);
  run(2, part2, false, 6446UL);
}
