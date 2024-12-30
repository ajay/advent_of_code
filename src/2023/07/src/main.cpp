// adventofcode.com/2023/day/7

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// #include <fmt/core.h>
#include <fmt/format.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

enum class Type {
  HighCard = 0,
  OnePair = 1,
  TwoPair = 2,
  ThreeOfAKind = 3,
  FullHouse = 4,
  FourOfAKind = 5,
  FiveOfAKind = 6,
  UNKNOWN = 0xFF,
};

struct Hand {
  std::string cards;
  std::string remap;
  size_t bid;
  Type type;
  uint32_t _reserved;
};

constexpr char remapToHex(const char c, bool weakJ) {
  switch (c) {
    case 'T':
      return 'A';
    case 'J':
      return weakJ ? '1' : 'B';
    case 'Q':
      return 'C';
    case 'K':
      return 'D';
    case 'A':
      return 'E';
    default:
      return c;
  }
};

std::vector<Hand> readFile(const std::string& path,
                           const std::function<Type(const std::string&)>& getType,
                           bool weakJ) {
  std::vector<Hand> hands{};

  auto lines = split(read(path), "\n");
  for (auto& line : lines) {
    auto [cardsString, bidString] = splitToPair(std::move(line));
    assert(cardsString.size() == 5);

    std::string remap{};
    std::transform(cardsString.begin(), cardsString.end(), std::back_inserter(remap),
                   [&weakJ](const auto& c) { return remapToHex(c, weakJ); });
    assert(remap.size() == 5);

    const auto type = getType(cardsString);
    hands.emplace_back(std::move(cardsString), std::move(remap), to<size_t>(std::move(bidString)),
                       type);
  }

  std::sort(hands.begin(), hands.end(), [](const auto& lhs, const auto& rhs) {
    if (lhs.type == rhs.type) {
      assert(lhs.remap.size() == rhs.remap.size());
      const auto comparison = lhs.remap.compare(rhs.remap);
      assert(comparison != 0);
      return comparison < 0;
    } else {
      return static_cast<size_t>(lhs.type) < static_cast<size_t>(rhs.type);
    }
  });

  return hands;
}

size_t sum(const std::string& path,
           const std::function<Type(const std::string&)>& getType,
           bool weakJ) {
  const auto hands = readFile(path, getType, weakJ);

  size_t sum = 0;
  for (size_t i = 0; i < hands.size(); ++i) {
    const auto& hand = hands[i];
    sum += (i + 1) * hand.bid;

    // fmt::print("sum: {:<12}    Hand: {} ({}), #J: {}, type: {}, bid: {}\n", sum, hand.cards,
    //            hand.remap, std::ranges::count(hand.cards, 'J'), static_cast<size_t>(hand.type),
    //            hand.bid);
  }

  return sum;
}

size_t part1(const std::string& path) {
  const auto getType = [](const std::string& cardsString) -> Type {
    std::unordered_map<char, size_t> counts;
    for (const auto& c : cardsString) {
      ++counts[c];
    }

    Type type = Type::UNKNOWN;
    if (counts.size() == 1) {
      type = Type::FiveOfAKind;
    } else if (counts.size() == 2) {
      const auto& [_ch, count] = *counts.begin();
      if ((count == 4) || (count == 1)) {
        type = Type::FourOfAKind;
      } else {
        type = Type::FullHouse;
      }
    } else if (counts.size() == 3) {
      for (const auto& [_ch, count] : counts) {
        if (count == 3) {
          type = Type::ThreeOfAKind;
          break;
        } else if (count == 2) {
          type = Type::TwoPair;
          break;
        }
      }
    } else if (counts.size() == 4) {
      type = Type::OnePair;
    } else {  // if (count.size() == 5) {
      type = Type::HighCard;
    }
    assert(type != Type::UNKNOWN);

    return type;
  };

  return sum(path, getType, false);
}

size_t part2(const std::string& path) {
  const auto getType = [](const std::string& cardsString) -> Type {
    std::unordered_map<char, size_t> counts;
    for (const auto& c : cardsString) {
      ++counts[c];
    }

    const auto cnt = [&counts](size_t countUnique, size_t countJokers = 0) {
      return (counts.size() == countUnique) &&
             (counts.count('J') ? (counts['J'] >= countJokers) : countJokers == 0);
    };

    // map of value (counts of a character) existing to resulting card type
    const auto mapJokerType =
        [&counts](const std::unordered_map<size_t, Type>& chCountToType) -> Type {
      for (const auto& [ch, count] : counts) {
        if (ch != 'J') {
          if (const auto it = chCountToType.find(count); it != chCountToType.end()) {
            return it->second;
          }
        }
      }

      assert(false);
      return Type::UNKNOWN;
    };

    Type type = Type::UNKNOWN;
    if (cnt(1) || (cnt(2, 1))) {  // AAAAA, AAAAJ, AAAJJ, AAJJJ, AJJJJ, JJJJJ
      type = Type::FiveOfAKind;
    } else if (cnt(2)) {  // AAAAB, AAABB, AABBB, ABBBB
      const auto& [_ch, count] = *counts.begin();
      if ((count == 4) || (count == 1)) {
        type = Type::FourOfAKind;
      } else {
        type = Type::FullHouse;
      }
    } else if (cnt(3, 2)) {  // ABJJJ, AABJJ, ABBJJ
      type = Type::FourOfAKind;
    } else if (cnt(3, 1)) {  // AAABJ, AABBJ, ABBBJ
      type = mapJokerType({
          {3, Type::FourOfAKind},
          {2, Type::FullHouse},
          {1, Type::FourOfAKind},
      });
    } else if (cnt(3)) {  // AAABC, AABBC, ABBBC, ABBCC,
      type = mapJokerType({
          {3, Type::ThreeOfAKind},
          {2, Type::TwoPair},
      });
    } else if (cnt(4, 1)) {  // ABCJJ
      return Type::ThreeOfAKind;
    } else if (cnt(4) || cnt(5, 1)) {  // ABCDD, ABCDJ
      type = Type::OnePair;
    } else {  // if (cnt(5)) {
      type = Type::HighCard;
    }
    assert(type != Type::UNKNOWN);

    return type;
  };

  return sum(path, getType, true);
}

}  // namespace

int main() {
  run(1, part1, true, 6440UL);
  run(1, part1, false, 250474325UL);
  run(2, part2, true, 5905UL);
  run(2, part2, false, 248909434UL);
}
