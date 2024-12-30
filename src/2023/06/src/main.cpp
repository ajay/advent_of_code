// adventofcode.com/2023/day/6

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

struct Race {
  size_t time;
  size_t distance;
};

std::vector<Race> readFile(const std::string& path) {
  auto [timesString, distancesString] = splitToPair(read(path), "\n");

  auto [timesKey, timesValues] = splitToPair(std::move(timesString), ":");
  assert(timesKey == "Time");
  const auto times = to<std::vector<size_t>>(split(std::move(timesValues)));

  auto [distancesKey, distancesValues] = splitToPair(std::move(distancesString), ":");
  assert(distancesKey == "Distance");
  const auto distances = to<std::vector<size_t>>(split(std::move(distancesValues)));

  assert(times.size() == distances.size());
  std::vector<Race> races;
  std::transform(times.begin(), times.end(), distances.begin(), std::back_inserter(races),
                 [](const auto& time, const auto& distance) {
                   return Race{.time = time, .distance = distance};
                 });

  return races;
}

size_t product(const std::vector<Race>& races) {
  size_t product = 1;
  for (const auto& race : races) {
    size_t winCounts = 0;
    for (size_t speed = 1; speed < race.time; ++speed) {
      if ((speed * (race.time - speed)) > race.distance) {
        ++winCounts;
      }
    }
    product *= winCounts;
  }

  return product;
}

size_t part1(const std::string& path) {
  return product(readFile(path));
}

size_t part2(const std::string& path) {
  const auto races = readFile(path);
  std::string time{};
  std::string distance{};
  for (const auto& race : races) {
    time += fmt::format("{}", race.time);
    distance += fmt::format("{}", race.distance);
  }

  const std::vector<Race> race = {{
      .time = to<size_t>(std::move(time)),
      .distance = to<size_t>(std::move(distance)),
  }};

  return product(race);
}

}  // namespace

int main() {
  run(1, part1, true, 288UL);
  run(1, part1, false, 1108800UL);
  run(2, part2, true, 71503UL);
  run(2, part2, false, 36919753UL);
}
