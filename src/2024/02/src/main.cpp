// adventofcode.com/2024/day/2

#include <algorithm>
#include <array>
#include <numeric>
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

std::vector<std::vector<size_t>> parse(const std::string& path) {
  std::vector<std::vector<size_t>> reports;

  auto data = readFile(path);
  for (auto&& line : data) {
    auto levels = splitTo<std::vector<size_t>>(std::move(line));
    reports.emplace_back(std::move(levels));
  }

  return reports;
}

bool checkReport(const std::vector<size_t>& report) {
  bool valid = std::is_sorted(report.begin(), report.end()) ||
               std::is_sorted(report.rbegin(), report.rend());

  for (size_t i = 1; valid && (i < report.size()); ++i) {
    const auto diff =
        std::abs(static_cast<ssize_t>(report[i]) - static_cast<ssize_t>(report[i - 1]));
    if (!((diff >= 1) && (diff <= 3))) {
      valid = false;
    }
  }

  return valid;
}

size_t checkReports(const std::string& path, auto&& checkFn) {
  auto reports = parse(path);
  return std::accumulate(
      reports.begin(), reports.end(), 0UL,
      [&checkFn](const auto& sum, const auto& report) { return sum + checkFn(report); });
}

size_t part1(const std::string& path) {
  return checkReports(path, checkReport);
}

size_t part2(const std::string& path) {
  return checkReports(path, [](const std::vector<size_t>& report) {
    bool valid = checkReport(report);

    for (size_t i = 0; !valid && (i < report.size()); ++i) {
      auto copy = report;
      copy.erase(std::next(copy.begin(), static_cast<ptrdiff_t>(i)));
      valid |= checkReport(copy);
    }

    return valid;
  });
}

int main() {
  run(1, part1, true, 2UL);
  run(1, part1, false, 383UL);
  run(2, part2, true, 4UL);
  run(2, part2, false, 436UL);
}
