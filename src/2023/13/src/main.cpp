// adventofcode.com/2023/day/12

#include <numeric>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

class Pattern final {
 public:
  Pattern(std::vector<std::string>&& grid) : grid_{std::move(grid)} {
    assert(grid_.size());
    assert(std::ranges::all_of(
        grid_, [this](const auto& line) { return line.size() == grid_[0].size(); }));
  }

  size_t reflection(size_t expectedBadness = 0) const {
    for (size_t row = 0; row < (grid_.size() - 1); ++row) {
      if (isHorizontalReflection(row) == expectedBadness) {
        return 100 * (row + 1);
      }
    }

    for (size_t col = 0; col < (grid_[0].size() - 1); ++col) {
      if (isVerticalReflection(col) == expectedBadness) {
        return col + 1;
      }
    }

    return 0;
  }

 private:
  size_t isHorizontalReflection(size_t row) const {
    assert(row < (grid_.size() - 1));
    size_t badness = 0;

    auto left = row;
    auto right = row + 1;
    while ((left < grid_.size()) && right < grid_.size()) {
      badness += std::inner_product(grid_[left].begin(), grid_[left].end(), grid_[right].begin(),
                                    size_t{}, std::plus<>(), std::not_equal_to<>());
      left--;
      right++;
    }

    return badness;
  }

  size_t isVerticalReflection(size_t col) const {
    const auto colSize = grid_[0].size();
    assert(col < colSize);
    size_t badness = 0;

    auto left = col;
    auto right = col + 1;
    while ((left < colSize) && (right < colSize)) {
      for (size_t i = 0; i < grid_.size(); ++i) {
        if (grid_[i][left] != grid_[i][right]) {
          ++badness;
        }
      }
      --left;
      ++right;
    }

    return badness;
  }

  std::vector<std::string> grid_;
};

std::vector<Pattern> readFile(const std::string& path) {
  auto patternsStrings = split(read(path), "\n\n", true);
  assert(patternsStrings.size());

  std::vector<Pattern> patterns{};
  std::ranges::transform(patternsStrings, std::back_inserter(patterns),
                         [](auto&& pattern) { return split(std::move(pattern), "\n"); });

  return patterns;
}

size_t sumReflections(const std::vector<Pattern>& patterns, size_t expectedBadness = 0) {
  return std::accumulate(patterns.begin(), patterns.end(), 0UL,
                         [&expectedBadness](const auto& sum, const auto& pattern) {
                           return sum + pattern.reflection(expectedBadness);
                         });
}

size_t part1(const std::string& path) {
  return sumReflections(readFile(path));
}

size_t part2(const std::string& path) {
  return sumReflections(readFile(path), 1);
}

}  // namespace

int main() {
  run(1, part1, true, 405UL);
  run(1, part1, false, 36041UL);
  run(2, part2, true, 400UL);
  run(2, part2, false, 35915UL);
}
