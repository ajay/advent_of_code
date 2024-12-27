// adventofcode.com/2024/day/20

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::map<std::pair<size_t, size_t>, size_t> distances(const std::vector<std::string>& grid,
                                                      const std::pair<size_t, size_t>& start,
                                                      const std::pair<size_t, size_t>& end) {
  std::map<std::pair<size_t, size_t>, size_t> distances = {{start, 0}};
  std::pair<size_t, size_t> current = start;
  std::pair<size_t, size_t> prev = {std::numeric_limits<size_t>::max(),
                                    std::numeric_limits<size_t>::max()};

  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  while (current != end) {
    for (const auto& dir : directions) {
      const auto& [rCur, cCur] = current;
      const auto& [rDir, cDir] = dir;
      const std::pair<size_t, size_t> next = {static_cast<ssize_t>(rCur) + rDir,
                                              static_cast<ssize_t>(cCur) + cDir};
      const auto& [rNext, cNext] = next;

      if (prev != next && grid[rNext][cNext] != '#') {
        distances[next] = distances[current] + 1;
        prev = current;
        current = next;
      }
    }
  }

  return distances;
}

size_t validShortcuts(std::vector<std::string> grid, size_t threshold, size_t maxBypass) {
  std::map<char, std::pair<size_t, size_t>> nodes;
  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
    for (size_t c = 0; c < grid[r].size(); ++c) {
      nodes[grid[r][c]] = {r, c};
    }
  }

  const std::pair<size_t, size_t> start = nodes['S'];
  const std::pair<size_t, size_t> end = nodes['E'];

  const auto distancesFromStart = distances(grid, start, end);
  const auto distancesFromEnd = distances(grid, end, start);

  size_t shortcuts = 0;
  const size_t maxDist = distancesFromEnd.at(start);
  for (const auto& [startCheat, startDist] : distancesFromStart) {
    for (const auto& [endCheat, endDist] : distancesFromEnd) {
      const size_t manhattan = static_cast<size_t>(llabs(static_cast<ssize_t>(startCheat.first) -
                                                         static_cast<ssize_t>(endCheat.first))) +
                               static_cast<size_t>(llabs(static_cast<ssize_t>(startCheat.second) -
                                                         static_cast<ssize_t>(endCheat.second)));
      const size_t bypassedLength = (startDist + endDist + manhattan);
      if ((manhattan >= 2) && (manhattan <= maxBypass) && (maxDist > bypassedLength) &&
          ((maxDist - bypassedLength) >= threshold)) {
        ++shortcuts;
      }
    }
  }

  return shortcuts;
}

std::map<size_t, size_t> count(const std::string& path,
                               const std::vector<size_t>& thresholds,
                               const size_t maxBypass) {
  const auto grid = readFile(path);

  std::map<size_t, size_t> results;
  std::transform(thresholds.begin(), thresholds.end(), std::inserter(results, results.end()),
                 [&grid, &maxBypass](const auto& threshold) {
                   return decltype(results)::value_type{threshold,
                                                        validShortcuts(grid, threshold, maxBypass)};
                 });

  return results;
}

std::map<size_t, size_t> part1(const std::string& path) {
  const bool example = (path == "data/example.txt");
  const std::vector<size_t> thresholds =
      example ? std::vector<size_t>{2, 4, 6, 8, 10, 12, 20, 36, 38, 40, 64, 100}
              : std::vector<size_t>{100};

  return count(path, thresholds, 2);
}

std::map<size_t, size_t> part2(const std::string& path) {
  const bool example = (path == "data/example.txt");
  const std::vector<size_t> thresholds =
      example ? std::vector<size_t>{50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 100}
              : std::vector<size_t>{100};

  return count(path, thresholds, 20);
}

int main() {
  run(1, part1, true,
      std::map<size_t, size_t>{
          {2, 44},   // There are 14 cheats that save 2 picoseconds
          {4, 30},   // There are 14 cheats that save 4 picoseconds
          {6, 16},   // There are 2 cheats that save 6 picoseconds
          {8, 14},   // There are 4 cheats that save 8 picoseconds
          {10, 10},  // There are 2 cheats that save 10 picoseconds
          {12, 8},   // There are 3 cheats that save 12 picoseconds
          {20, 5},   // There is one cheat that saves 20 picoseconds
          {36, 4},   // There is one cheat that saves 36 picoseconds
          {38, 3},   // There is one cheat that saves 38 picoseconds
          {40, 2},   // There is one cheat that saves 40 picoseconds
          {64, 1},   // There is one cheat that saves 64 picoseconds
          {100, 0},
      });
  run(1, part1, false, std::map<size_t, size_t>{{100, 1307}});
  run(2, part2, true,
      std::map<size_t, size_t>{
          {50, 285},  // There are 32 cheats that save 50 picoseconds
          {52, 253},  // There are 31 cheats that save 52 picoseconds
          {54, 222},  // There are 29 cheats that save 54 picoseconds
          {56, 193},  // There are 39 cheats that save 56 picoseconds
          {58, 154},  // There are 25 cheats that save 58 picoseconds
          {60, 129},  // There are 23 cheats that save 60 picoseconds
          {62, 106},  // There are 20 cheats that save 62 picoseconds
          {64, 86},   // There are 19 cheats that save 64 picoseconds
          {66, 67},   // There are 12 cheats that save 66 picoseconds
          {68, 55},   // There are 14 cheats that save 68 picoseconds
          {70, 41},   // There are 12 cheats that save 70 picoseconds
          {72, 29},   // There are 22 cheats that save 72 picoseconds
          {74, 7},    // There are 4 cheats that save 74 picoseconds
          {76, 3},    // There are 3 cheats that save 76 picoseconds
          {100, 0},
      });
  run(2, part2, false, std::map<size_t, size_t>{{100, 986545}});
}
