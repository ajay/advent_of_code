// adventofcode.com/{{ year }}/day/{{ day }}

#include <string>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path));
}

size_t part1(const std::string& path) {
  return readFile(path).size();
}

size_t part2(const std::string& path) {
  return readFile(path).size();
}

}  // namespace

int main() {
  run(1, part1, true, 0UL);
  run(1, part1, false, 0UL);
  run(2, part2, true, 0UL);
  run(2, part2, false, 0UL);
}
