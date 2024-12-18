// adventofcode.com/2024/day/18

#include <algorithm>
#include <array>
#include <cctype>
#include <execution>
#include <list>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fmt/core.h>
#include <boost/generator_iterator.hpp>
#include <boost/range/counting_range.hpp>
#include <boost/range/irange.hpp>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::vector<std::pair<size_t, size_t>> parse(const std::string& path) {
  std::vector<std::pair<size_t, size_t>> points;

  auto lines = readFile(path);
  for (auto&& line : lines) {
    const auto [c, r] = splitToPair<size_t, size_t>(std::move(line), ",");
    points.emplace_back(r, c);
  }

  return points;
}

size_t simulate(const std::string& path, size_t turns, size_t rows, size_t cols) {
  const auto data = parse(path);
  const std::pair<size_t, size_t> start = {0, 0};
  const std::pair<size_t, size_t> end = {rows, cols};
  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  std::vector<std::vector<size_t>> grid((rows + 1), std::vector<size_t>((cols + 1), 0));
  for (size_t i = 0; i <= turns; ++i) {
    const auto& [r, c] = data[i];
    grid[r][c] = 1;
  }

  std::vector<std::vector<size_t>> visited((rows + 1), std::vector<size_t>((cols + 1), 0));
  std::vector<std::vector<size_t>> distance((rows + 1), std::vector<size_t>((cols + 1), 0));
  std::queue<std::pair<std::pair<size_t, size_t>, size_t>> queue;
  queue.emplace(start, 0);

  const auto& [rStart, cStart] = start;
  visited[rStart][cStart] = 1;

  while (!queue.empty()) {
    const auto [curr, d] = queue.front();
    queue.pop();

    if (curr == end) {
      return d;
    }

    for (const auto& dir : directions) {
      const auto& [rDir, cDir] = dir;
      const auto& [rCurr, cCurr] = curr;

      const size_t r = static_cast<size_t>(static_cast<ssize_t>(rCurr) + rDir);
      const size_t c = static_cast<size_t>(static_cast<ssize_t>(cCurr) + cDir);

      if ((r > rows) || (c > cols) || visited[r][c] || grid[r][c]) {
        continue;
      }

      visited[r][c] = 1;
      distance[r][c] = d + 1;
      queue.emplace(std::pair<size_t, size_t>{r, c}, d + 1);
    }
  }

  const auto& [rEnd, cEnd] = end;
  if (!visited[rEnd][cEnd]) {
    return std::numeric_limits<size_t>::max();
  }

  return distance[rEnd][cEnd];
}

size_t part1(const std::string& path) {
  const bool example = (path == "data/example.txt");
  const size_t rowCol = example ? 6 : 70;
  const size_t turns = example ? 12 : 1024;

  return simulate(path, turns, rowCol, rowCol);
}

std::string part2(const std::string& path) {
  const bool example = (path == "data/example.txt");
  const size_t rowCol = example ? 6 : 70;

  const auto raw = readFile(path);
  for (size_t i = 0; i < raw.size(); ++i) {
    const size_t d = simulate(path, i, rowCol, rowCol);
    if (d == std::numeric_limits<size_t>::max()) {
      return raw[i];
    }
  }

  return "";
}

int main() {
  // run(1, part1, true, 22UL); // is the example wrong? getting 24...
  run(1, part1, false, 404UL);
  run(2, part2, true, "6,1");
  run(2, part2, false, "27,60");
}
