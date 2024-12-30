// adventofcode.com/2024/day/9

#include <cassert>
#include <cctype>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>

// #include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::vector<ssize_t> parse(const std::string& path) {
  const auto raw = readFile(path);
  assert(raw.size() == 1);
  const auto& data = raw[0];

  std::vector<ssize_t> disk;
  disk.reserve(data.size() * 9);  // max digit

  for (size_t i = 0; i < data.size(); ++i) {
    const auto& ch = data[i];
    assert(std::isdigit(ch));
    const auto num = to<size_t>(std::string{ch});

    for (size_t x = 0; x < num; ++x) {
      disk.emplace_back(((i % 2) == 0) ? static_cast<ssize_t>(i / 2) : -1);
    }
  }

  return disk;
}

size_t checksum(const std::vector<ssize_t>& filesystem) {
  size_t sum = 0;
  for (size_t i = 0; i < filesystem.size(); ++i) {
    if (filesystem[i] >= 0) {
      sum += (static_cast<size_t>(filesystem[i]) * i);
    }
  }

  return sum;
}

size_t moveBlocks(const std::string& path) {
  auto data = parse(path);
  // fmt::println("data: {}", data);

  size_t l = 0;
  size_t r = (data.size() - 1);
  while (l < r) {
    while (data[l] >= 0) {
      ++l;
    }

    while (data[r] < 0) {
      --r;
    }

    if (l < r) {
      std::swap(data[l], data[r]);
    }
  }
  // fmt::println("data: {}", data);

  return checksum(data);
}

size_t moveFiles(const std::string& path) {
  auto data = parse(path);

  size_t emptyL = 0;
  size_t emptyR = emptyL;
  size_t fileR = (data.size() - 1);
  size_t fileL = fileR;

  while (fileL > 0) {
    while (data[emptyL] >= 0) {
      ++emptyL;
    }
    emptyR = emptyL;
    while (data[emptyR + 1] < 0) {
      ++emptyR;
    }
    size_t esize = (emptyR - emptyL) + 1;

    while (data[fileR] < 0) {
      --fileR;
    }
    fileL = fileR;
    while (data[fileL - 1] == data[fileR]) {
      --fileL;
    }
    size_t fsize = (fileR - fileL) + 1;

    if ((emptyL >= data.size()) || (emptyR > fileL)) {
      emptyL = 0;
      emptyR = 0;
      fileR = (fileL - 1);
      continue;
    }

    if (esize >= fsize) {
      const auto val = data[fileL];
      // fmt::println("..move {}", val);
      for (size_t e = 0; e < fsize; ++e) {
        data[emptyL + e] = val;
      }
      for (size_t f = 0; f < fsize; ++f) {
        data[fileL + f] = -1;
      }
      emptyL = 0;
      emptyR = 0;
    } else {
      emptyL = emptyR + 1;
    }
  }
  // fmt::println("data: {}", data);

  return checksum(data);
}

size_t part1(const std::string& path) {
  return moveBlocks(path);
}

size_t part2(const std::string& path) {
  return moveFiles(path);
}

int main() {
  run(1, part1, true, 1928UL);
  run(1, part1, false, 6398252054886UL);
  run(2, part2, true, 2858UL);
  run(2, part2, false, 6415666220005UL);
}
