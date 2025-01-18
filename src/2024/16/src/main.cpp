// adventofcode.com/2024/day/16

#include <cassert>
#include <cstddef>
#include <deque>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <sys/types.h>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::pair<size_t, size_t> find(const std::vector<std::string>& grid, const char target) {
  std::pair<size_t, size_t> rc = {std::numeric_limits<size_t>::max(),
                                  std::numeric_limits<size_t>::max()};

  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
    for (size_t c = 0; c < grid[0].size(); ++c) {
      if (grid[r][c] == target) {
        assert(rc == (std::pair<size_t, size_t>{std::numeric_limits<size_t>::max(),
                                                std::numeric_limits<size_t>::max()}));
        rc = {r, c};
      }
    }
  }

  return rc;
}

size_t dijkstra(const std::string& path,
                bool part2 = false,
                const char startCh = 'S',
                const char endCh = 'E') {
  const std::vector<std::pair<ssize_t, ssize_t>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  const auto grid = readFile(path);
  const auto& [rowStart, colStart] = find(grid, startCh);

  using RowColDirection = std::tuple<size_t, size_t, size_t>;
  using DistanceRowColDirection = std::tuple<size_t, size_t, size_t, size_t>;

  std::map<RowColDirection, std::vector<RowColDirection>> history;
  std::map<RowColDirection, size_t> distance = {{{rowStart, colStart, 0}, 0}};
  std::priority_queue<DistanceRowColDirection, std::vector<DistanceRowColDirection>, std::greater<>>
      pqueue;
  pqueue.push({0, rowStart, colStart, 0});

  while (!pqueue.empty()) {
    const auto [dist, row, col, dir] = pqueue.top();
    pqueue.pop();
    // fmt::println("row: {}, col: {}, dist: {}, dir: {}", row, col, dist, dir);

    if (grid[row][col] == endCh) {
      if (!part2) {
        return dist;
      } else {
        break;
      }
    }

    for (size_t newDir = 0; newDir < 4; ++newDir) {
      const auto& [deltaR, deltaC] = directions[newDir];
      const size_t newRow = static_cast<size_t>(static_cast<ssize_t>(row) + deltaR);
      const size_t newCol = static_cast<size_t>(static_cast<ssize_t>(col) + deltaC);
      const size_t newDist = dist + ((newDir != dir) ? 1001 : 1);

      if (!((newRow >= grid.size()) || (newCol >= grid[0].size()) ||
            (grid[newRow][newCol] == '#'))) {
        const auto current = RowColDirection{newRow, newCol, newDir};
        if (distance.find(current) == distance.end() || (newDist < distance[current])) {
          distance[current] = newDist;
          pqueue.emplace(newDist, newRow, newCol, newDir);
          history[current].clear();
          history[current].emplace_back(row, col, dir);
        } else if (newDist == distance[current]) {
          history[current].emplace_back(row, col, dir);
        }
      }
    }
  }

  const auto [rowEnd, colEnd] = find(grid, endCh);
  std::queue<RowColDirection> queue({
      {rowEnd, colEnd, 0},
      {rowEnd, colEnd, 1},
      {rowEnd, colEnd, 2},
      {rowEnd, colEnd, 3},
  });
  std::set<std::pair<size_t, size_t>> unique;

  while (!queue.empty()) {
    const auto [row, col, dir] = queue.front();
    queue.pop();

    unique.emplace(row, col);
    for (const auto& predecessor : history[{row, col, dir}]) {
      queue.push(predecessor);
    }
  }

  return unique.size();
}

size_t part1(const std::string& path) {
  return dijkstra(path);
}

size_t part2(const std::string& path) {
  return dijkstra(path, true);
}

int main() {
  run(1, part1, true, 7036UL, "data/example.txt");
  run(1, part1, true, 11048UL, "data/example2.txt");
  run(1, part1, false, 92432UL);
  run(2, part2, true, 45UL, "data/example.txt");
  run(2, part2, true, 64UL, "data/example2.txt");
  run(2, part2, false, 458UL);
}
