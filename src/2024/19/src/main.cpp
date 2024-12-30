// adventofcode.com/2024/day/19

#include <cassert>
#include <cstddef>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

struct Towels {
  std::vector<std::string> patterns;
  std::vector<std::string> designs;
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n\n", true);
}

Towels parse(const std::string& path) {
  auto patternsDesignsStrs = readFile(path);
  assert(patternsDesignsStrs.size() == 2);

  const Towels towels = {
      .patterns = split(std::move(patternsDesignsStrs[0]), ", "),
      .designs = split(std::move(patternsDesignsStrs[1]), "\n"),
  };

  // fmt::println("patterns: {}", towels.patterns);
  // fmt::println("designs:  {}", towels.designs);

  return towels;
}

size_t check(std::string_view design,
             std::unordered_map<std::string_view, size_t>& cache,
             const std::vector<std::string>& patterns) {
  if (const auto it = cache.find(design); it != cache.end()) {
    return it->second;
  }

  size_t possible = 0;
  for (const auto& pattern : patterns) {
    if (design.starts_with(pattern)) {
      possible += check(design.substr(pattern.size()), cache, patterns);
    }
  }

  assert(cache.count(design) == 0);
  cache.emplace(design, possible);
  return possible;
}

size_t count(const std::string& path, bool single = true) {
  const auto towels = parse(path);

  std::unordered_map<std::string_view, size_t> cache = {{"", 1}};
  return std::accumulate(towels.designs.begin(), towels.designs.end(), 0UL,
                         [&towels, &cache, &single](const auto& sum, const auto& design) {
                           const auto possible = check(design, cache, towels.patterns);
                           // fmt::println("{} {}", design, possible);
                           return sum + (single ? (possible > 0) : possible);
                         });
}

size_t part1(const std::string& path) {
  return count(path);
}

size_t part2(const std::string& path) {
  return count(path, false);
}

int main() {
  run(1, part1, true, 6UL);
  run(1, part1, false, 285UL);
  run(2, part2, true, 16UL);
  run(2, part2, false, 636483903099279UL);
}
