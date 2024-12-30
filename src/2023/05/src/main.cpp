// adventofcode.com/2023/day/5

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// #include <fmt/core.h>
#include <fmt/format.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

struct Range {
  size_t startDestination;
  size_t startSource;
  size_t length;
};

struct AlmanacMap {
  std::string from;
  std::string to;
  std::vector<Range> ranges;

  size_t mapVal(size_t val) const {
    for (const auto& range : ranges) {
      if (val >= range.startSource && (val < (range.startSource + range.length))) {
        val += (range.startDestination - range.startSource);
        break;
      }
    }

    return val;
  }
};

struct Almanac {
  std::vector<size_t> seeds;
  std::vector<AlmanacMap> maps;
};

Almanac readFile(const std::string& path) {
  Almanac almanac{};

  auto sections = split(read(path), "\n\n", true);
  assert(sections.size());

  auto& sectionSeeds = sections[0];
  auto [seedString, seedNumsString] = splitToPair(std::move(sectionSeeds), ":");
  assert(seedString == "seeds");
  almanac.seeds = to<decltype(almanac.seeds)>(split(std::move(seedNumsString)));
  sections.erase(sections.begin());

  for (auto&& section : sections) {
    auto [sectionFromTo, sectionRanges] = splitToPair(std::move(section), " map:\n", true);
    const auto [sectionFrom, sectionTo] = splitToPair(std::move(sectionFromTo), "-to-", true);

    auto sectionRangesSplit = split(std::move(sectionRanges), "\n");
    std::vector<Range> ranges{};
    std::transform(sectionRangesSplit.begin(), sectionRangesSplit.end(), std::back_inserter(ranges),
                   [](auto& range) {
                     auto nums = split(std::move(range));
                     assert(nums.size() == 3);
                     return Range{.startDestination = to<size_t>(std::move(nums[0])),
                                  .startSource = to<size_t>(std::move(nums[1])),
                                  .length = to<size_t>(std::move(nums[2]))};
                   });

    almanac.maps.emplace_back(AlmanacMap{
        .from = std::move(sectionFrom),
        .to = std::move(sectionTo),
        .ranges = std::move(ranges),
    });
  }

  return almanac;
}

size_t lowestLocation(const Almanac& almanac, std::unordered_map<size_t, size_t>& cache) {
  auto lowestLocation = std::numeric_limits<size_t>::max();

  for (size_t i = 0; i < almanac.seeds.size(); ++i) {
    if ((i % 50000000) == 0) {
      // fmt::print("Processing seed {} / {}\n", i, almanac.seeds.size());
    }

    const auto& seed = almanac.seeds[i];
    std::string currentFrom = "seed";
    size_t currentVal = seed;

    if (const auto it = cache.find(seed); it != cache.end()) {
      // fmt::print("Found cached seed: '{}', val: '{}'", seed, it->second);
      lowestLocation = std::min(it->second, lowestLocation);
      continue;
    }

    for (const auto& map : almanac.maps) {
      if (map.from == currentFrom) {
        currentFrom = map.to;
        currentVal = map.mapVal(currentVal);
      }

      if (currentFrom == "location") {
        lowestLocation = std::min(currentVal, lowestLocation);
        break;
      }
    }

    cache[seed] = lowestLocation;
  }

  return lowestLocation;
}

size_t part1(const std::string& path) {
  std::unordered_map<size_t, size_t> cache{};
  return lowestLocation(readFile(path), cache);
}

size_t part2(const std::string& path) {
  auto almanac = readFile(path);

  assert(almanac.seeds.size() % 2 == 0);
  std::vector<size_t> newSeeds{};
  for (size_t i = 0; i < almanac.seeds.size(); i += 2) {
    const auto start = almanac.seeds[i];
    const auto length = almanac.seeds[i + 1];
    // fmt::print("Processing range: {} + {} ({} / {})\n", start, length, i + 2,
    // almanac.seeds.size());
    for (size_t j = 0; j < length; ++j) {
      newSeeds.emplace_back(start + j);
    }
  }

  almanac.seeds = std::move(newSeeds);
  std::unordered_map<size_t, size_t> cache{};
  // fmt::print("Starting to process part2\n");
  return lowestLocation(almanac, cache);
}

}  // namespace

int main() {
  run(1, part1, true, 35UL);
  run(1, part1, false, 484023871UL);
  run(2, part2, true, 46UL);

  // Brute force: works, but requires ~120GB of RAM & ~2 hours
  // run(2, part2, false, 46294175UL);
}
