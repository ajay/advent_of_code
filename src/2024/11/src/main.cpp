// adventofcode.com/2024/day/11

#include <algorithm>
#include <array>
#include <cctype>
#include <list>
#include <numeric>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::list<size_t> parse(const std::string& path) {
  auto data = readFile(path);
  assert(data.size() == 1);
  return splitTo<std::list<size_t>>(std::move(data[0]), " ");
}

// size_t blink(const std::string& path, size_t turns) {
//   auto nums = parse(path);

//   for (size_t turn = 0; turn < turns; ++turn) {
//     // fmt::println("{:2} {}", turn, nums);
//     // fmt::println("{}", turn);
//     for (auto it = nums.begin(); it != nums.end(); ++it) {
//       if (*it == 0) {
//         *it = 1;
//       } else if (const auto str = std::to_string(*it); str.size() % 2 == 0) {
//         auto lhs = str.substr(0, str.size() / 2);
//         auto rhs = str.substr(str.size() / 2);

//         *it = to<size_t>(std::move(lhs));
//         nums.insert(it, to<size_t>(std::move(rhs)));
//       } else {
//         *it *= 2024;
//       }
//     }
//   }

//   return nums.size();
// }

size_t blink(const std::string& path, size_t turns) {
  const auto initial = parse(path);
  std::map<size_t, size_t> nums;

  for (const auto& num : initial) {
    if (nums.find(num) != nums.end()) {
      nums[num] = 1;
    } else {
      ++nums[num];
    }
  }

  for (size_t turn = 0; turn < turns; ++turn) {
    std::map<size_t, size_t> newNums;
    // fmt::println("{:2} {}\n\n", turn, nums);
    const auto insertOrAdd = [&newNums](const auto& num, const auto& count) {
      if (newNums.find(num) == newNums.end()) {
        newNums[num] = count;
      } else {
        newNums[num] += count;
      }
    };

    for (const auto& [num, count] : nums) {
      if (num == 0) {
        insertOrAdd(num + 1, count);
      } else if (const auto str = std::to_string(num); str.size() % 2 == 0) {
        auto lhs = str.substr(0, str.size() / 2);
        auto rhs = str.substr(str.size() / 2);

        insertOrAdd(to<size_t>(std::move(lhs)), count);
        insertOrAdd(to<size_t>(std::move(rhs)), count);
      } else {
        insertOrAdd(num * 2024, count);
      }
    }

    nums = std::move(newNums);
  }

  return std::accumulate(nums.begin(), nums.end(), 0UL, [](const auto& sum, const auto& kv) {
    const auto& [num, count] = kv;
    return sum + count;
  });
}

size_t part1(const std::string& path) {
  return blink(path, 25);
}

size_t part2(const std::string& path) {
  return blink(path, 75);
}

int main() {
  run(1, part1, true, 55312UL);
  run(1, part1, false, 183435UL);
  // run(2, part2, true, 81UL);
  run(2, part2, false, 218279375708592UL);
}
