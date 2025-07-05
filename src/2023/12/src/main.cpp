// adventofcode.com/2023/day/12

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#if defined __has_include && __has_include("fmt/base.h")
#include <fmt/base.h>
#endif
#include <fmt/format.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

enum class Condition : char {
  Operational = '.',
  Damaged = '#',
  Unknown = '?',
};

class Record final {
 public:
  Record(std::string&& conditions, std::vector<size_t>&& damagedGroups)
      : conditions_{std::move(conditions)}, damagedGroups_{std::move(damagedGroups)} {}

  size_t possibleUnknownCombinations() const {
    assert(conditions_.find_first_not_of(
               std::string{to<char>(Condition::Operational), to<char>(Condition::Damaged),
                           to<char>(Condition::Unknown)}) == std::string::npos);

    return countUnknownCombinations(conditions_, damagedGroups_);
  }

  std::string print() const {
    return fmt::format("[ .conditions_ = {:20} .damagedGroups_ = {} ]", conditions_,
                       damagedGroups_);
  }

  void unfold(size_t expansion = 5) {
    assert(expansion == 5);  // figure out way to consteval create format string using expansion
    conditions_ = fmt::format("{}?{}?{}?{}?{}", conditions_, conditions_, conditions_, conditions_,
                              conditions_);

    const auto copy = damagedGroups_;
    for (size_t i = 0; i < (expansion - 1); ++i) {
      std::copy(copy.begin(), copy.end(), std::back_inserter(damagedGroups_));
    }
  }

 private:
  static size_t countUnknownCombinations(const std::string& line, const std::vector<size_t>& runs);
  static std::map<std::pair<std::string, std::vector<size_t>>, size_t> combinationsCache_;

  std::string conditions_;
  std::vector<size_t> damagedGroups_;
};

std::map<std::pair<std::string, std::vector<size_t>>, size_t> Record::combinationsCache_{};
size_t Record::countUnknownCombinations(const std::string& line, const std::vector<size_t>& runs) {
  std::pair<std::string, std::vector<size_t>> key = {line, runs};
  if (const auto it = combinationsCache_.find(key); it != combinationsCache_.end()) {
    return it->second;
  }

  const auto returnAndCache = [cachedKey{std::move(key)}](size_t ret) {
    if (const auto it = combinationsCache_.find(cachedKey); it != combinationsCache_.end()) {
      assert(false);
    }

    combinationsCache_.emplace(cachedKey, ret);
    return ret;
  };

  if (line.size() == 0) {
    if (runs.size() == 0) {
      return returnAndCache(1);
    }
    return returnAndCache(0);
  }

  if (runs.size() == 0) {
    for (size_t i = 0; i < line.size(); ++i) {
      if ((i < line.size()) && line[i] == to<char>(Condition::Damaged)) {
        return returnAndCache(0);
      }
    }
    return returnAndCache(1);
  }

  const auto sumRuns = std::accumulate(runs.begin(), runs.end(), 0UL,
                                       [](const auto& sum, const auto& run) { return sum + run; });
  if (line.size() < (sumRuns + runs.size() - 1)) {
    return returnAndCache(0);
  }

  assert(runs.size() >= 1);
  if (line[0] == to<char>(Condition::Operational)) {
    return returnAndCache(countUnknownCombinations(line.substr(1), runs));
  }

  if (line[0] == to<char>(Condition::Damaged)) {
    const auto run = runs.front();
    const std::vector<size_t> leftover(std::next(runs.begin()), runs.end());
    for (size_t i = 0; i < run; ++i) {
      if ((i < line.size()) && line[i] == to<char>(Condition::Operational)) {
        return returnAndCache(0);
      }
    }

    if ((run < line.size()) && (line[run] == to<char>(Condition::Damaged))) {
      return returnAndCache(0);
    }

    return returnAndCache(
        countUnknownCombinations(line.substr(std::min(line.size(), run + 1)), leftover));
  }

  return returnAndCache(
      countUnknownCombinations(std::string{to<char>(Condition::Damaged)} + line.substr(1), runs) +
      countUnknownCombinations(std::string{to<char>(Condition::Operational)} + line.substr(1),
                               runs));
}

std::vector<Record> readFile(const std::string& path, bool log = false) {
  std::vector<Record> records{};

  auto lines = split(read(path), "\n");
  for (auto&& line : lines) {
    auto [conditions, damagedGroupsString] = splitToPair(std::move(line));
    auto damagedGroups_ = splitTo<std::vector<size_t>>(std::move(damagedGroupsString), ",");

    records.emplace_back(std::move(conditions), std::move(damagedGroups_));
  }

  if (log) {
    fmt::print("[\n");
    for (const auto& record : records) {
      fmt::print("  {},\n", record.print());
    }
    fmt::print("]\n");
  }

  return records;
}

size_t sumOfArrangements(const std::vector<Record>& records) {
  return std::accumulate(records.begin(), records.end(), 0UL,
                         [](const auto& sum, const auto& record) mutable {
                           return sum + record.possibleUnknownCombinations();
                         });
}

size_t part1(const std::string& path) {
  return sumOfArrangements(readFile(path));
}

size_t part2(const std::string& path) {
  auto records = readFile(path);
  for (auto& record : records) {
    record.unfold();
  }

  return sumOfArrangements(records);
}

}  // namespace

int main() {
  run(1, part1, true, 21UL);
  run(1, part1, false, 7670UL);
  run(2, part2, true, 525152UL);
  run(2, part2, false, 157383940585037UL);
}
