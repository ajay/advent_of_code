// adventofcode.com/2024/day/5

#include <algorithm>
#include <array>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

struct Data {
  std::vector<std::pair<size_t, size_t>> rules;
  std::vector<std::vector<size_t>> updates;
};

Data readFile(const std::string& path) {
  Data data;

  auto [rulesStr, updatesStr] = splitToPair(read(path), "\n\n", true);

  auto rules = split(std::move(rulesStr), "\n");
  for (auto&& rule : rules) {
    auto [lhs, rhs] = splitToPair<size_t, size_t>(std::move(rule), "|");
    data.rules.emplace_back(std::pair<size_t, size_t>{lhs, rhs});
  }

  auto updates = split(std::move(updatesStr), "\n");
  for (auto&& update : updates) {
    data.updates.emplace_back(splitTo<std::vector<size_t>>(std::move(update), ","));
  }

  return data;
}

size_t check(const std::string& path, bool correct = false) {
  auto data = readFile(path);
  size_t sum = 0;

  for (auto& update : data.updates) {
    bool valid = !correct;

    bool allPassing = true;
    do {
      allPassing = true;
      for (const auto& [lhs, rhs] : data.rules) {
        if (const auto itL = std::find(update.begin(), update.end(), lhs),
            itR = std::find(update.begin(), update.end(), rhs);
            itL != update.end() && itR != update.end()) {
          if (!(std::distance(update.begin(), itR) > std::distance(update.begin(), itL))) {
            if (!correct) {
              valid = false;
            } else {
              allPassing = false;
              valid = true;
              const auto val = *itR;
              update.erase(itR);
              update.insert(itL, val);
            }
          }
        }
      }
    } while (!allPassing);

    if (valid) {
      assert(update.size() % 2 == 1);
      sum += update[update.size() / 2];
    }
  }

  return sum;
}

size_t part1(const std::string& path) {
  return check(path);
}

size_t part2(const std::string& path) {
  return check(path, true);
}

int main() {
  run(1, part1, true, 143UL);
  run(1, part1, false, 4872UL);
  run(2, part2, true, 123UL);
  run(2, part2, false, 5564UL);
}
