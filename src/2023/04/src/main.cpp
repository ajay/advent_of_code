// adventofcode.com/2023/day/4

#include <array>
#include <numeric>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

struct Card {
  size_t index;
  std::vector<size_t> winningNumbers{};
  std::vector<size_t> yourNumbers{};
};

std::vector<Card> readFile(const std::string& path) {
  std::vector<Card> cards{};

  auto lines = split(read(path), "\n");
  for (auto&& line : lines) {
    auto [lineFirstHalf, lineSecondHalf] = splitToPair(std::move(line), ":");

    const auto [cardString, numberString] = splitToPair(std::move(lineFirstHalf));
    assert(cardString == "Card");
    const auto cardNumber = to<size_t>(numberString);
    auto [winningNumbersString, yourNumbersString] = splitToPair(std::move(lineSecondHalf), "|");

    auto winningNumbers = to<std::vector<size_t>>(split(std::move(winningNumbersString)));
    auto yourNumbers = to<std::vector<size_t>>(split(std::move(yourNumbersString)));
    std::sort(winningNumbers.begin(), winningNumbers.end());
    std::sort(yourNumbers.begin(), yourNumbers.end());

    cards.emplace_back(cardNumber, std::move(winningNumbers), std::move(yourNumbers));
  }

  return cards;
}

size_t part1(const std::string& path) {
  size_t sum = 0;

  const auto cards = readFile(path);
  for (const auto& card : cards) {
    std::vector<size_t> intersection{};
    std::set_intersection(card.winningNumbers.begin(), card.winningNumbers.end(),
                          card.yourNumbers.begin(), card.yourNumbers.end(),
                          std::inserter(intersection, intersection.begin()));

    sum += intersection.size() ? (1 << (intersection.size() - 1)) : 0;
  }

  return sum;
}

size_t part2(const std::string& path) {
  const auto cards = readFile(path);
  std::vector<size_t> copies(cards.size(), 1);

  // for (const auto& card : cards) {
  for (size_t i = 0; i < cards.size(); ++i) {
    auto& card = cards[i];
    std::vector<size_t> intersection{};
    std::set_intersection(card.winningNumbers.begin(), card.winningNumbers.end(),
                          card.yourNumbers.begin(), card.yourNumbers.end(),
                          std::inserter(intersection, intersection.begin()));

    for (size_t j = 0; j < copies[i]; ++j) {
      for (size_t k = card.index;
           k < std::min((card.index + intersection.size()), cards.back().index); ++k) {
        ++copies[k];
      }
    }
  }

  return std::accumulate(copies.begin(), copies.end(), 0UL);
}

}  // namespace

int main() {
  run(1, part1, true, 13UL);
  run(1, part1, false, 22193UL);
  run(2, part2, true, 30UL);
  run(2, part2, false, 5625994UL);
}
