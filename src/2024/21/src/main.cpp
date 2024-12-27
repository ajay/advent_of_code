// adventofcode.com/2024/day/21

#include <algorithm>
#include <cstddef>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

struct Keypads {
  std::vector<std::string> numpad;
  std::vector<std::string> dirpad;
  std::map<char, std::pair<size_t, size_t>> digits;
  std::map<char, std::pair<size_t, size_t>> directions;
  size_t dirpads;
};

Keypads keypads(size_t dirpads) {
  const std::vector<std::string> numpad = {
      // clang-format off
    "789",
    "456",
    "123",
    " 0A",
      // clang-format on
  };
  const std::vector<std::string> dirpad = {
      // clang-format off
    " ^A",
    "<v>",
      // clang-format on
  };

  std::map<char, std::pair<size_t, size_t>> digits;
  for (size_t r = 0; r < numpad.size(); ++r) {
    assert(numpad[r].size() == numpad[0].size());
    for (size_t c = 0; c < numpad[r].size(); ++c) {
      if (numpad[r][c] != ' ') {
        digits[numpad[r][c]] = {r, c};
      }
    }
  }

  std::map<char, std::pair<size_t, size_t>> directions;
  for (size_t r = 0; r < dirpad.size(); ++r) {
    assert(dirpad[r].size() == dirpad[0].size());
    for (size_t c = 0; c < dirpad[r].size(); ++c) {
      if (dirpad[r][c] != ' ') {
        directions[dirpad[r][c]] = {r, c};
      }
    }
  }

  return {
      .numpad = numpad,
      .dirpad = dirpad,
      .digits = digits,
      .directions = directions,
      .dirpads = dirpads,
  };
}

std::vector<std::string> pathsBetween(const std::vector<std::string>& grid,
                                      const std::pair<size_t, size_t>& to,
                                      const std::pair<size_t, size_t>& from) {
  const auto& [rTo, cTo] = to;
  const auto& [rFrom, cFrom] = from;

  if (!grid[rTo][cTo] || grid[rTo][cTo] == ' ') {
    return {};
  } else if (to == from) {
    return {"A"};
  }

  std::vector<std::string> paths;
  const auto insertPaths = [&paths](auto&& pathsNew, char dir) {
    std::transform(pathsNew.begin(), pathsNew.end(), pathsNew.begin(),
                   [&dir](const auto& path) { return fmt::format("{:c}{:s}", dir, path); });
    paths.insert(paths.end(), std::make_move_iterator(pathsNew.begin()),
                 std::make_move_iterator(pathsNew.end()));
  };

  if (rTo > rFrom) {
    insertPaths(pathsBetween(grid, {rTo - 1, cTo}, from), '^');
  } else if (rTo < rFrom) {
    insertPaths(pathsBetween(grid, {rTo + 1, cTo}, from), 'v');
  }

  if (cTo > cFrom) {
    insertPaths(pathsBetween(grid, {rTo, cTo - 1}, from), '<');
  } else if (cTo < cFrom) {
    insertPaths(pathsBetween(grid, {rTo, cTo + 1}, from), '>');
  }

  return paths;
}

size_t shortestPathLayer(char a,
                         char b,
                         size_t layer,
                         std::map<std::pair<std::pair<char, char>, size_t>, size_t>& cache,
                         const Keypads& kp) {
  if (layer == (kp.dirpads + 1)) {
    return 1;  // press upper layer button
  }

  const std::pair<std::pair<char, char>, size_t> key = {{a, b}, layer};
  if (cache.contains(key)) {
    return cache[key];
  }

  size_t shortestPath = std::numeric_limits<size_t>::max();
  const auto paths =
      (layer == 0 ? pathsBetween(kp.numpad, kp.digits.at(a), kp.digits.at(b))
                  : pathsBetween(kp.dirpad, kp.directions.at(a), kp.directions.at(b)));

  for (const auto& path : paths) {
    size_t sum = shortestPathLayer('A', path[0], layer + 1, cache, kp);

    for (size_t i = 0; i < path.size() - 1; ++i) {
      sum += shortestPathLayer(path[i], path[i + 1], layer + 1, cache, kp);
    }

    shortestPath = std::min(shortestPath, sum);
  }

  cache[key] = shortestPath;
  return shortestPath;
}

size_t shortestPathLayer(char a, char b, size_t layer, const Keypads& kp) {
  std::map<std::pair<std::pair<char, char>, size_t>, size_t> cache;
  return shortestPathLayer(a, b, layer, cache, kp);
}

size_t shortestPathCode(const std::string& code, const Keypads& kp) {
  size_t sum = shortestPathLayer('A', code[0], 0, kp);
  for (size_t i = 0; i < code.size() - 1; ++i) {
    sum += shortestPathLayer(code[i], code[i + 1], 0, kp);
  }

  return sum;
}

size_t shortestPathCodes(const std::vector<std::string>& codes, const Keypads& kp) {
  return std::accumulate(codes.begin(), codes.end(), 0UL, [&kp](const auto& sum, const auto& code) {
    return sum + shortestPathCode(code, kp) * to<size_t>(code.substr(0, 3));
  });
}

size_t complexity(const std::string& path, size_t dirpads) {
  return shortestPathCodes(readFile(path), keypads(dirpads));
}

size_t part1(const std::string& path) {
  return complexity(path, 2);
}

size_t part2(const std::string& path) {
  return complexity(path, 25);
}

int main() {
  run(1, part1, true, 126384UL);
  run(1, part1, false, 171596UL);
  // run(2, part2, true, 154115708116294UL);
  run(2, part2, false, 209268004868246UL);
}
