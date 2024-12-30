// adventofcode.com/2023/day/14

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

namespace {

class Platform final {
 public:
  Platform(std::vector<std::string>&& grid) : grid_{std::move(grid)} {}

  enum class Direction {
    North,
    South,
    West,
    East,
  };

  void print
      [[maybe_unused]] (const std::string& msg = "", const std::string& msgStem = "platform") {
    fmt::print("{}: [\n  {}\n]\n", fmt::format("{} {}: ", msgStem, msg), fmt::join(grid_, "\n  "));
  }

  size_t load() const {
    size_t sum = 0;
    for (size_t r = 0; r < grid_.size(); ++r) {
      for (size_t c = 0; c < grid_[r].size(); ++c) {
        if (grid_[r][c] == 'O') {
          sum += (grid_.size() - r);
        }
      }
    }

    return sum;
  }

  void tilt(const Direction& direction = Direction::North) {
    bool movement = true;
    const auto checkAndSwap = [this, &movement](const std::pair<size_t, size_t>& curr,
                                                const std::pair<size_t, size_t>& prev) {
      const auto& [x1, y1] = curr;
      const auto& [x2, y2] = prev;

      if ((grid_[y1][x1] == 'O') && (grid_[y2][x2] == '.')) {
        movement = true;
        grid_[y2][x2] = 'O';
        grid_[y1][x1] = '.';
      }
    };

    while (movement) {
      movement = false;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-default"
#endif

      switch (direction) {
        case Direction::North:
          for (size_t r = 1; r < grid_.size(); ++r) {
            for (size_t c = 0; c < grid_[r].size(); ++c) {
              checkAndSwap({c, r}, {c, r - 1});
            }
          }
          break;
        case Direction::South:
          for (size_t r = grid_.size() - 2; r < grid_.size(); --r) {
            for (size_t c = 0; c < grid_[r].size(); ++c) {
              checkAndSwap({c, r}, {c, r + 1});
            }
          }
          break;
        case Direction::West:
          for (size_t c = 1; c < grid_[0].size(); ++c) {
            for (size_t r = 0; r < grid_.size(); ++r) {
              checkAndSwap({c, r}, {c - 1, r});
            }
          }
          break;
        case Direction::East:
          for (size_t c = grid_[0].size() - 2; c < grid_[0].size(); --c) {
            for (size_t r = 0; r < grid_.size(); ++r) {
              checkAndSwap({c, r}, {c + 1, r});
            }
          }
          break;
      }

#ifdef __clang__
#pragma clang diagnostic pop
#endif
    }
  }

  void spin() {
    tilt(Direction::North);
    tilt(Direction::West);
    tilt(Direction::South);
    tilt(Direction::East);
  }

  const std::vector<std::string>& grid() { return grid_; }

 private:
  std::vector<std::string> grid_;
};

Platform readFile(const std::string& path) {
  auto platform = split(read(path), "\n");

  assert(platform.size());
  assert(std::ranges::all_of(platform, [&platform](const auto& row) {
    return row.size() == platform[0].size() &&
           std::ranges::all_of(row, [](const auto& c) { return c == 'O' || c == '.' || c == '#'; });
  }));

  return {std::move(platform)};
}

size_t part1(const std::string& path) {
  auto platform = readFile(path);
  // platform.print("before");
  platform.tilt();
  // platform.print("after");
  return platform.load();
}

size_t part2(const std::string& path) {
  auto platform = readFile(path);
  std::map<std::vector<std::string>, size_t> lastSeen{};

  constexpr size_t kNumCycles = 1000000000;
  bool fastForward = false;
  for (size_t i = 0; i < kNumCycles; ++i) {
    platform.spin();

    if (!fastForward) {
      if (const auto it = lastSeen.find(platform.grid()); it != lastSeen.end()) {
        const auto& [_, lastIndex] = *it;
        const auto period = i - lastIndex;
        i += ((kNumCycles - i) / period) * period;
        fastForward = true;
      } else {
        lastSeen.emplace(std::vector<std::string>{platform.grid()}, i);
      }
    }

    // fmt::print("load: {:<10} {:>10} / {:<10} ({:.1f}%)\n", platform.load(), i, kNumCycles,
    //            static_cast<double>(i) * (100. / static_cast<double>(kNumCycles)));
  }

  return platform.load();
}

}  // namespace

int main() {
  run(1, part1, true, 136UL);
  run(1, part1, false, 109661UL);
  run(2, part2, true, 64UL);
  run(2, part2, false, 90176UL);
}
