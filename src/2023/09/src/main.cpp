// adventofcode.com/2023/day/9

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <sys/types.h>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

namespace {

std::vector<std::vector<ssize_t>> readFile(const std::string& path) {
  auto lines = split(read(path), "\n");

  std::vector<std::vector<ssize_t>> histories{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(histories),
                 [](auto&& line) { return splitTo<std::vector<ssize_t>>(std::move(line)); });

  assert(std::all_of(histories.begin(), histories.end(), [&histories](const auto& history) {
    return history.size() == histories[0].size();
  }));

  // fmt::print("Histories: [\n  {}\n]\n", fmt::join(histories, ",\n  "));
  return histories;
}

template <class T>
T calculateNext(const std::vector<T>& histories, bool extrapolateFront) {
  assert(histories.size() >= 2);

  std::vector<std::remove_cvref_t<decltype(histories)>> steps{histories};

  const auto allZeroes = [&steps]() {
    assert(steps.size());
    return std::all_of(steps.back().begin(), steps.back().end(),
                       [](const auto& e) { return e == T{}; });
  };

  while (!allZeroes()) {
    typename decltype(steps)::value_type step{};
    const auto last = steps.back();
    assert(last.size() >= 2);

    for (size_t i = 0; i < (last.size() - 1); ++i) {
      step.emplace_back(last[i + 1] - last[i]);
    }
    steps.emplace_back(std::move(step));
  }

  if (extrapolateFront) {
    steps.back().emplace_back(T{});
    for (ssize_t i = static_cast<ssize_t>(steps.size()) - 2; i >= 0; --i) {
      const auto z = static_cast<size_t>(i);
      steps[z].emplace_back(steps[z].back() + steps[z + 1].back());
    }
  } else {
    steps.back().insert(steps.back().begin(), T{});
    for (ssize_t i = static_cast<ssize_t>(steps.size()) - 2; i >= 0; --i) {
      const auto z = static_cast<size_t>(i);
      steps[z].insert(steps[z].begin(), steps[z].front() - steps[z + 1].front());
    }
  }
  // fmt::print("Steps: [\n  {}\n]\n", fmt::join(steps, ",\n  "));

  return extrapolateFront ? steps.front().back() : steps.front().front();
};

ssize_t extrapolate(const std::string& path, bool extrapolateFront) {
  const auto histories = readFile(path);
  return std::accumulate(histories.begin(), histories.end(), 0,
                         [&extrapolateFront](const auto& sum, const auto& history) {
                           return sum + calculateNext(history, extrapolateFront);
                         });
}

ssize_t part1(const std::string& path) {
  return extrapolate(path, true);
}

ssize_t part2(const std::string& path) {
  return extrapolate(path, false);
}

}  // namespace

int main() {
  run(1, part1, true, 114L);
  run(1, part1, false, 1980437560L);
  run(2, part2, true, 2L);
  run(2, part2, false, 977L);
}
