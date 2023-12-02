// adventofcode.com/2023/day/1

#include <algorithm>
#include <array>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n");
}

std::pair<std::pair<char, ssize_t>, std::pair<char, ssize_t>> firstAndLastDigit(
    const std::string_view& s) {
  constexpr std::string_view kDigits{"0123456789"};

  std::pair<std::pair<char, ssize_t>, std::pair<char, ssize_t>> ret{
      {0, std::numeric_limits<ssize_t>::max()},
      {0, std::numeric_limits<ssize_t>::min()}};
  auto& [first, last] = ret;

  if (const auto firstOf = s.find_first_of(kDigits);
      firstOf != std::string::npos) {
    first = {s[firstOf], firstOf};
  }

  if (const auto lastOf = s.find_last_of(kDigits);
      lastOf != std::string::npos) {
    last = {s[lastOf], lastOf};
  }

  return ret;
}

std::pair<std::pair<char, ssize_t>, std::pair<char, ssize_t>> firstAndLastWord(
    const std::string_view& s) {
  constexpr std::array<std::string_view, 9> kWords = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
  };
  constexpr char kBaseOrdinal = '1';

  std::pair<std::pair<char, ssize_t>, std::pair<char, ssize_t>> ret{
      {0, std::numeric_limits<ssize_t>::max()},
      {0, std::numeric_limits<ssize_t>::min()}};
  auto& [first, last] = ret;

  for (size_t i = 0; !first.first && (i < s.size()); ++i) {
    for (size_t j = 0; j < kWords.size(); ++j) {
      if (s.substr(i, kWords[j].size()) == kWords[j]) {
        first = {kBaseOrdinal + j, i};
        break;
      }
    }
  }

  for (size_t i = s.size() - 1; !last.first && (i != 0); --i) {
    for (size_t j = 0; j < kWords.size(); ++j) {
      if (s.substr(i, kWords[j].size()) == kWords[j]) {
        last = {kBaseOrdinal + j, i};
        break;
      }
    }
  }

  return ret;
}

size_t solve(const std::string& path, bool stringsAsNumbers = false) {
  const auto lines = readFile(path);
  std::vector<size_t> nums{};
  std::transform(
      lines.begin(), lines.end(), std::back_inserter(nums),
      [&stringsAsNumbers](const auto& line) {
        char first{};
        char last{};
        const auto& [firstDigit, lastDigit] = firstAndLastDigit(line);
        const auto& [firstDigitChar, firstDigitIdx] = firstDigit;
        const auto& [lastDigitChar, lastDigitIdx] = lastDigit;

        if (!stringsAsNumbers) {
          first = firstDigitChar;
          last = lastDigitChar;
        } else {
          const auto& [firstWord, lastWord] = firstAndLastWord(line);
          const auto& [firstWordChar, firstWordIdx] = firstWord;
          const auto& [lastWordChar, lastWordIdx] = lastWord;

          first =
              (firstDigitIdx < firstWordIdx) ? firstDigitChar : firstWordChar;
          last = ((lastDigitIdx > lastWordIdx) ? lastDigitChar : lastWordChar);
        }

        // fmt::print("Line: {}, first: {}, last: {}\n", line, first, last);

        return std::stoull(std::string{first} + last);
      });

  return std::accumulate(nums.begin(), nums.end(), 0UL);
}

size_t part1(const std::string& path) {
  return solve(path);
}

size_t part2(const std::string& path) {
  return solve(path, true);
}

int main() {
  run(1, part1, true, 142UL);
  run(1, part1, false, 55971UL);
  run(2, part2, true, 281UL, "data/example2.txt");
  run(2, part2, false, 54719UL);
}
