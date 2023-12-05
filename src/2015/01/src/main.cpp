// adventofcode.com/2015/day/1

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/run.h"

// @param targetFloor Ignore if 0, else break once the floor is found.
// @return Final floor or index if targetFloor != 0.
ssize_t deliver(const std::string& path, ssize_t targetFloor = 0) {
  const auto floors = read(path);

  ssize_t floor = 0;
  for (size_t i = 0; i < floors.size(); ++i) {
    const auto& move = floors[i];

    if (move == '(') {
      ++floor;
    } else if (move == ')') {
      --floor;
    } else {
      throw std::invalid_argument(fmt::format("Unexpected character: {}", move));
    }

    if (targetFloor && (targetFloor == floor)) {
      floor = static_cast<ssize_t>(i + 1);
      break;
    }
  }

  return floor;
}

ssize_t part1(const std::string& path) {
  return deliver(path);
}

ssize_t part2(const std::string& path) {
  return deliver(path, -1);
}

int main() {
  run(1, part1, false, 138L);
  run(2, part2, false, 1771L);
}
