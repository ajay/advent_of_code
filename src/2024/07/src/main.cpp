// adventofcode.com/2024/day/7

#include <algorithm>
#include <array>
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

struct Calibration final {
  size_t target;
  std::vector<size_t> nums;
};

enum class Operator {
  Add,
  Multiply,
  Concatenate,
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::vector<Calibration> parse(const std::string& path) {
  auto data = readFile(path);

  std::vector<Calibration> calibrations;
  for (auto&& line : data) {
    auto [targetStr, numsStr] = splitToPair(std::move(line), ":");
    auto nums = splitTo<std::vector<size_t>>(std::move(numsStr));
    calibrations.emplace_back(to<size_t>(std::move(targetStr)), std::move(nums));
  }

  return calibrations;
}

bool evaluate(const std::vector<size_t>& nums, size_t target, const std::vector<Operator>& ops) {
  if (nums.size() == 1) {
    return nums[0] == target;
  }

  for (const auto& op : ops) {
    size_t value = 0;
    switch (op) {
      case Operator::Add:
        value = nums[0] + nums[1];
        break;
      case Operator::Multiply:
        value = nums[0] * nums[1];
        break;
      case Operator::Concatenate:
        value = to<size_t>(std::to_string(nums[0]) + std::to_string(nums[1]));
        break;
      default:
        assert(false);
        break;
    }

    std::vector<size_t> numsNew = {value};
    if (nums.size() > 2) {
      numsNew.insert(numsNew.end(), std::next(nums.begin(), 2), nums.end());
    }

    if (evaluate(numsNew, target, ops)) {
      return true;
    }
  }

  return false;
}

size_t solve(const std::string& path, const std::vector<Operator>& ops) {
  const auto calibrations = parse(path);

  size_t result = 0;
  for (const auto& [target, nums] : calibrations) {
    if (evaluate(nums, target, ops)) {
      result += target;
    }
  }

  return result;
}

size_t part1(const std::string& path) {
  return solve(path, {Operator::Add, Operator::Multiply});
}

size_t part2(const std::string& path) {
  return solve(path, {Operator::Add, Operator::Multiply, Operator::Concatenate});
}

int main() {
  run(1, part1, true, 3749UL);
  run(1, part1, false, 2941973819040UL);
  run(2, part2, true, 11387UL);
  run(2, part2, false, 249943041417600UL);
}
