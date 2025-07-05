// adventofcode.com/2023/day/16

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined __has_include && __has_include("fmt/base.h")
#include <fmt/base.h>
#endif
#include <fmt/core.h>  // IWYU pragma: keep

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

namespace {

enum class Tile : char {
  Empty = '.',
  MirrorBackward = '\\',
  MirrorForward = '/',
  SplitterHorizontal = '-',
  SplitterVertical = '|',
};

enum class Direction : char { Up = '^', Down = '_', Left = '<', Right = '>' };

struct Beam {
  size_t x;
  size_t y;
  Direction direction;
  uint8_t _reserved[7]{};

  std::optional<Beam> update(const Tile& tile) {
    auto maybeNewBeam = updateDirection(tile);
    updatePosition();
    return maybeNewBeam;
  }

  std::optional<Beam> updateDirection(const Tile& tile) {
    std::optional<Beam> beamSplit{};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-default"
#endif

    switch (tile) {
      case Tile::Empty:
        break;
      case Tile::MirrorBackward:
        switch (direction) {
          case Direction::Up:
            direction = Direction::Left;
            break;
          case Direction::Down:
            direction = Direction::Right;
            break;
          case Direction::Left:
            direction = Direction::Up;
            break;
          case Direction::Right:
            direction = Direction::Down;
            break;
        }
        break;
      case Tile::MirrorForward:
        switch (direction) {
          case Direction::Up:
            direction = Direction::Right;
            break;
          case Direction::Down:
            direction = Direction::Left;
            break;
          case Direction::Left:
            direction = Direction::Down;
            break;
          case Direction::Right:
            direction = Direction::Up;
            break;
        }
        break;
      case Tile::SplitterHorizontal:
        switch (direction) {
          case Direction::Up:
          case Direction::Down:
            direction = Direction::Left;
            beamSplit = {.x = x, .y = y, .direction = Direction::Right};
            break;
          case Direction::Left:
          case Direction::Right:
            break;
        }
        break;
      case Tile::SplitterVertical:
        switch (direction) {
          case Direction::Up:
          case Direction::Down:
            break;
          case Direction::Left:
          case Direction::Right:
            direction = Direction::Up;
            beamSplit = {.x = x, .y = y, .direction = Direction::Down};
            break;
        }
        break;
    }

    return beamSplit;
  }

  void updatePosition(bool log = false) {
    if (log) {
      fmt::print("direction: '{}', position: ({}, {})  ->  ", static_cast<char>(direction), x, y);
    }

    switch (direction) {
      case Direction::Up:
        --y;
        break;
      case Direction::Down:
        ++y;
        break;
      case Direction::Left:
        --x;
        break;
      case Direction::Right:
        ++x;
        break;
    }

#ifdef __clang__
#pragma clang diagnostic pop
#endif

    if (log) {
      fmt::print("({}, {})\n", x, y);
    }
  }
};

std::vector<std::string> readFile(const std::string& path) {
  const auto grid = split(read(path), "\n");

  assert(grid.size());
  assert(std::ranges::all_of(grid, [&grid](const auto& row) { return row.size() == grid.size(); }));

  return grid;
}

size_t energized(const std::vector<std::string>& grid, const Beam& beamStarting) {
  const std::unordered_map<Direction, bool> directionsVisited = {
      {Direction::Up, false},
      {Direction::Down, false},
      {Direction::Left, false},
      {Direction::Right, false},
  };

  std::vector<std::vector<std::unordered_map<Direction, bool>>> visited(
      grid.size(), decltype(visited)::value_type(grid[0].size(), directionsVisited));
  std::vector<Beam> beams{beamStarting};

  while (beams.size()) {
    const auto [beamBeginNew, beamEndNew] =
        std::ranges::remove_if(beams, [&grid, &visited](const auto& beam) {
          return ((beam.x >= grid[0].size()) || (beam.y >= grid.size()) ||
                  (visited[beam.y][beam.x][beam.direction]));
        });
    beams.erase(beamBeginNew, beamEndNew);

    for (size_t i = 0; i < beams.size(); ++i) {
      auto& beam = beams[i];
      const auto& [x, y, direction, _] = beam;
      visited[y][x][direction] = true;
      if (auto maybeNewBeam = beam.update(static_cast<Tile>(grid[y][x])); maybeNewBeam) {
        beams.emplace_back(std::move(*maybeNewBeam));
      }
    }
  }

  // clang-format off
  return std::accumulate(visited.begin(), visited.end(), size_t{}, [](const auto& sum, const auto& row) {
    return sum + std::accumulate(row.begin(), row.end(), size_t{}, [](const auto& sumRow, const auto& e) {
      return sumRow + std::ranges::any_of(e, [](const auto& pair) {
        return pair.second;
      });
    });
  });
  // clang-format on
}

size_t part1(const std::string& path) {
  return energized(readFile(path), {.x = 0, .y = 0, .direction = Direction::Right});
}

size_t part2(const std::string& path) {
  const auto grid = readFile(path);

  std::vector<Beam> beams{};
  for (size_t y = 0; y < grid.size(); ++y) {
    beams.emplace_back(0, y, Direction::Right);
    beams.emplace_back(grid[0].size() - 1, y, Direction::Left);
  }
  for (size_t x = 0; x < grid[0].size(); ++x) {
    beams.emplace_back(x, 0, Direction::Down);
    beams.emplace_back(x, grid.size() - 1, Direction::Up);
  }

  std::vector<size_t> energies{};
  std::ranges::transform(beams, std::back_inserter(energies),
                         [&grid](const auto& beam) { return energized(grid, beam); });

  return *std::ranges::max_element(energies);
}

}  // namespace

int main() {
  run(1, part1, true, 46UL);
  run(1, part1, false, 7543UL);
  run(2, part2, true, 51UL);
  run(2, part2, false, 8231UL);
}
