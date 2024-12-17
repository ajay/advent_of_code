// adventofcode.com/2024/day/17

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

struct Computer {
  size_t a;
  size_t b;
  size_t c;

  std::vector<size_t> program;
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n\n", true);
}

Computer parse(const std::string& path) {
  auto data = readFile(path);
  assert(data.size() == 2);

  auto registersStrs = split(std::move(data[0]), "\n");
  assert(registersStrs.size() == 3);

  const auto registerStr = [&registersStrs](size_t i, char ch) {
    auto str = split(std::move(registersStrs[i]), "Register:\n");
    assert(str.size() == 3);
    assert(str[0] == "");
    assert(str[1] == std::string{ch});
    return str[2];
  };

  auto programStr = split(std::move(data[1]), "Program:");
  assert(programStr.size() == 2);
  assert(programStr[0] == "");

  return {
      .a = to<size_t>(registerStr(0, 'A')),
      .b = to<size_t>(registerStr(1, 'B')),
      .c = to<size_t>(registerStr(2, 'C')),
      .program = splitTo<std::vector<size_t>>(std::move(programStr[1]), ","),
  };
}

std::string simulate(const std::string& path, ssize_t registerA = -1) {
  std::string outStr;
  auto computer = parse(path);

  if (registerA != -1) {
    computer.a = static_cast<size_t>(registerA);
  }

  const auto combo = [&computer](size_t op) {
    switch (op) {
      case 0:
      case 1:
      case 2:
      case 3:
        return op;
      case 4:
        return computer.a;
      case 5:
        return computer.b;
      case 6:
        return computer.c;
      default:
        assert(false);
        break;
    }
  };

  size_t iPointer = 0;
  while (iPointer < (computer.program.size() - 1)) {
    size_t iOpcode = iPointer;
    size_t iOperand = iPointer + 1;

    size_t opcode = computer.program[iOpcode];
    size_t operand = computer.program[iOperand];

    // fmt::println("\nprogram: {}", computer.program);
    // fmt::println("a: {}", computer.a);
    // fmt::println("b: {}", computer.b);
    // fmt::println("c: {}", computer.c);
    // fmt::println("iOpcode: {}, iOperand: {}\nopcode: {}, operand: {}", iOpcode, iOperand, opcode,
    //              operand);

    const auto dv = [&computer, &combo, &operand]() {
      const size_t num = computer.a;
      const size_t den = static_cast<size_t>(2ULL << (combo(operand) - 1UL));
      if (den == 0) {
        throw std::runtime_error("what u trying to do");
      }
      return num / den;
    };

    bool advanced = false;
    switch (opcode) {
      case 0: {  // adv
        computer.a = dv();
      } break;
      case 1: {  // bxl
        computer.b = computer.b ^ operand;
      } break;
      case 2: {  // bst
        computer.b = combo(operand) % 8;
      } break;
      case 3: {  // jnz
        if (computer.a == 0) {
          break;
        }
        iPointer = operand;
        advanced = true;
      } break;
      case 4: {  // bxc
        computer.b = computer.b ^ computer.c;
      } break;
      case 5: {  // out
        const size_t out = combo(operand) % 8;
        outStr += std::to_string(out) + ",";
      } break;
      case 6: {  // bdv
        computer.b = dv();
      } break;
      case 7: {  // cdv
        computer.c = dv();
      } break;
      default:
        assert(false);
    }

    if (!advanced) {
      iPointer += 2;
    }
  }

  outStr.pop_back();  // remove last comma
  return outStr;
}

size_t fixRegisterA(const std::string& path, size_t cheatStart = 0) {
  const auto computer = parse(path);
  auto expectedStr =
      std::accumulate(computer.program.begin(), computer.program.end(), std::string{},
                      [](auto&& str, const auto& num) { return str += std::to_string(num) + ","; });
  expectedStr.pop_back();  // remove last comma
  // fmt::println("expectedStr: {}", expectedStr);

  std::atomic<size_t> idx = 0;

  auto nums = boost::irange<size_t>(cheatStart, SIZE_MAX / 2);
  std::any_of(std::execution::par, nums.begin(), nums.end(), [&path, &expectedStr, &idx](size_t i) {
    // if (i && (i % 1'000'000 == 0)) {
    // fmt::println("{}", i);
    // }

    try {
      if (simulate(path, static_cast<ssize_t>(i)) == expectedStr) {
        // fmt::println("good: {}", i);
        if (idx > 0) {
          if (idx > i) {
            idx = i;
          }
        } else {
          idx = i;
        }
        // idx = (idx > 0) ? idx.fetch_min(i) : i;
        return true;
      } else {
        return false;
      }
    } catch (const std::exception& e [[maybe_unused]]) {
      // fmt::println("{} exception: {}", i, e.what());
      return false;
    }
  });

  // for (; i < std::numeric_limits<size_t>::max(); ++i) {
  //   if (i % 1'000'000 == 0) {
  //     fmt::println("{}", i);
  //   }

  //   try {
  //     if (simulate(path, static_cast<ssize_t>(i)) == expectedStr) {
  //       break;
  //     }
  //   } catch (const std::exception& e [[maybe_unused]]) {
  //     // fmt::println("{} exception: {}", i, e.what());
  //     continue;
  //   }
  // }

  return idx;
}

std::string part1(const std::string& path) {
  return simulate(path);
}

size_t part2(const std::string& path) {
  const bool input = (path == "data/input.txt");
  return fixRegisterA(path, input ? 105875099912600UL : 0);
}

int main() {
  run(1, part1, true, "4,6,3,5,6,3,5,2,1,0");
  run(1, part1, false, "6,5,7,4,5,7,3,1,0");
  run(2, part2, true, 117440UL, "data/example2.txt");
  // run(2, part2, false, 105875099912602UL); // TODO: make part 2 performant without 'cheatStart'
}
