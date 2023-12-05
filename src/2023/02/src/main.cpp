// adventofcode.com/2023/day/2

#include <algorithm>
#include <map>
#include <numeric>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

enum class Color : size_t {
  Red,
  Green,
  Blue,
};

Color toColor(const std::string_view& colorString) {
  if (colorString == "red") {
    return Color::Red;
  } else if (colorString == "green") {
    return Color::Green;
  } else if (colorString == "blue") {
    return Color::Blue;
  } else {
    throw std::runtime_error(fmt::format("Color not found: {}", colorString));
  }
}

struct RevealedCube {
  size_t num;
  Color color;
};

struct Turn {
  using Moves = std::vector<RevealedCube>;
  std::vector<Moves> moves;
};

struct Game {
  std::vector<Turn> turns;
};

std::map<size_t, Game> readFile(const std::string& path) {
  std::map<size_t, Game> games{};

  auto lines = split(read(path), "\n");
  for (auto&& line : lines) {
    auto [lineFirstHalf, lineSecondHalf] = splitToPair(std::move(line), ":");

    // TODO: add helper splitTo<>() called with different types as:
    // splitTo<std::pair<std::string, size_t>>()
    const auto [gameString, gameNumberString] = splitToPair(std::move(lineFirstHalf));
    assert(gameString == "Game");
    const auto gameNumber = to<size_t>(gameNumberString);

    auto [gameIt, gameItStatus] = games.emplace(gameNumber, Game{});
    assert(gameItStatus);
    auto& game = *gameIt;

    auto turnStrings = split(std::move(lineSecondHalf), ";");
    for (auto& turnString : turnStrings) {
      auto& turn = game.second.turns.emplace_back();

      auto movesStrings = split(std::move(turnString), ",");
      for (auto& moveString : movesStrings) {
        auto& move = turn.moves.emplace_back();
        const auto [moveNumStr, moveColorStr] = splitToPair(std::move(moveString));
        const auto moveNum = to<size_t>(moveNumStr);
        const auto moveColor = toColor(moveColorStr);
        move.emplace_back(moveNum, moveColor);
      }
    }
  }

  return games;
}

size_t part1(const std::string& path) {
  const std::unordered_map<Color, ssize_t> kTotals = {
      {Color::Red, 12},
      {Color::Green, 13},
      {Color::Blue, 14},
  };

  size_t sum = 0;
  const auto games = readFile(path);
  for (const auto& [index, game] : games) {
    bool possible = true;

    for (const auto& turn : game.turns) {
      for (const auto& move : turn.moves) {
        auto balances = kTotals;
        for (const auto& cube : move) {
          balances[cube.color] -= cube.num;
        }

        if (!std::all_of(balances.begin(), balances.end(),
                         [](const auto& balance) { return balance.second >= 0; })) {
          possible = false;
        }
      }
    }

    if (possible) {
      sum += index;
    }
  }

  return sum;
}

size_t part2(const std::string& path) {
  size_t sum = 0;

  const auto games = readFile(path);
  for (const auto& [_index, game] : games) {
    std::unordered_map<Color, size_t> counts{
        {Color::Red, 0},
        {Color::Green, 0},
        {Color::Blue, 0},
    };
    for (const auto& turn : game.turns) {
      for (const auto& move : turn.moves) {
        for (const auto& cube : move) {
          counts[cube.color] = std::max(counts[cube.color], cube.num);
        }
      }
    }

    if (std::all_of(counts.begin(), counts.end(),
                    [](const auto& balance) { return balance.second >= 0; })) {
      sum += std::accumulate(counts.begin(), counts.end(), 1UL,
                             [](const auto& prev, auto& val) { return prev * val.second; });
    }
  }

  return sum;
}

}  // namespace

int main() {
  run(1, part1, true, 8UL);
  run(1, part1, false, 2716UL);
  run(2, part2, true, 2286UL);
  run(2, part2, false, 72227UL);
}
