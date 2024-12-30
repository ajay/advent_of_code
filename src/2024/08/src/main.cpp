// adventofcode.com/2024/day/8

#include <cassert>
#include <cstddef>
#include <string>
#include <unordered_map>
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

size_t count(std::vector<std::string> grid,
             const std::unordered_map<char, std::vector<std::pair<size_t, size_t>>>& positions,
             bool extended = false) {
  size_t nodes = 0;

  const auto valid = [&grid](ssize_t r, ssize_t c) {
    return (r >= 0) && (r < static_cast<ssize_t>(grid.size())) && (c >= 0) &&
           (c < (static_cast<ssize_t>(grid[0].size())));
  };

  for (const auto& [_, points] : positions) {
    if (extended) {
      nodes += points.size();
    }

    for (size_t p1 = 0; p1 < points.size(); ++p1) {
      for (size_t p2 = p1 + 1; p2 < points.size(); ++p2) {
        const auto p1r = static_cast<ssize_t>(points[p1].first);
        const auto p1c = static_cast<ssize_t>(points[p1].second);
        const auto p2r = static_cast<ssize_t>(points[p2].first);
        const auto p2c = static_cast<ssize_t>(points[p2].second);

        const auto dr = p1r - p2r;
        const auto dc = p1c - p2c;
        auto nr = p1r + dr;
        auto nc = p1c + dc;

        const auto get = [&grid](ssize_t r, ssize_t c) {
          return grid[static_cast<size_t>(r)][static_cast<size_t>(c)];
        };

        const auto set = [&grid](ssize_t r, ssize_t c, char ch) {
          grid[static_cast<size_t>(r)][static_cast<size_t>(c)] = ch;
        };

        if (extended) {
          while (valid(nr, nc)) {
            if (get(nr, nc) == '.') {
              ++nodes;
              set(nr, nc, '#');
            }

            nr += dr;
            nc += dc;
          }
        } else if (valid(nr, nc) && get(nr, nc) != '#') {
          ++nodes;
          set(nr, nc, '#');
        }

        nr = p2r - dr;
        nc = p2c - dc;

        if (extended) {
          while (valid(nr, nc)) {
            if (get(nr, nc) == '.') {
              ++nodes;
              set(nr, nc, '#');
            }

            nr -= dr;
            nc -= dc;
          }
        } else if (valid(nr, nc) && get(nr, nc) != '#') {
          ++nodes;
          set(nr, nc, '#');
        }
      }
    }
  }

  return nodes;
}

size_t count(const std::string& path, bool extended = false) {
  const auto data = readFile(path);

  std::unordered_map<char, std::vector<std::pair<size_t, size_t>>> positions;
  for (size_t r = 0; r < data.size(); ++r) {
    assert(data[r].size() == data[0].size());
    for (size_t c = 0; c < data[r].size(); ++c) {
      if (data[r][c] != '.') {
        positions[data[r][c]].push_back({r, c});
      }
    }
  }

  return count(data, positions, extended);
}

size_t part1(const std::string& path) {
  return count(path);
}

size_t part2(const std::string& path) {
  return count(path, true);
}

int main() {
  run(1, part1, true, 14UL);
  run(1, part1, false, 354UL);
  run(2, part2, true, 34UL);
  run(2, part2, false, 1263UL);
}
