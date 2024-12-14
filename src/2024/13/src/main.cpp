// adventofcode.com/2024/day/13

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

struct Game final {
  struct Point final {
    size_t x;
    size_t y;
  };

  Point deltaA;
  Point deltaB;
  Point prize;
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n\n", true);
}

std::vector<Game> parse(const std::string& path) {
  auto gamesStr = readFile(path);
  std::vector<Game> games{};

  for (auto&& gameStr : gamesStr) {
    const auto button = [](std::string&& line, char expected) {
      auto parts = split(std::move(line), " :,+");
      assert(parts.size() == 6);
      assert(parts[0] == "Button");
      assert(parts[1] == std::string{expected});
      assert(parts[2] == "X");
      assert(parts[4] == "Y");

      return Game::Point{to<size_t>(std::move(parts[3])), to<size_t>(std::move(parts[5]))};
    };

    const auto prize = [](std::string&& line) {
      auto parts = split(std::move(line), " :,=");
      assert(parts.size() == 5);
      assert(parts[0] == "Prize");
      assert(parts[1] == "X");
      assert(parts[3] == "Y");

      return Game::Point{to<size_t>(std::move(parts[2])), to<size_t>(std::move(parts[4]))};
    };

    auto lines = split(std::move(gameStr), "\n");
    assert(lines.size() == 3);
    games.emplace_back(Game{.deltaA = button(std::move(lines[0]), 'A'),
                            .deltaB = button(std::move(lines[1]), 'B'),
                            .prize = prize(std::move(lines[2]))});
  }

  return games;
}

// size_t cheapestCost(const Game& game, size_t extraPrize = 0, size_t costA = 3, size_t costB = 1)
// {
//   const auto prizeX = game.prize.x + extraPrize;
//   const auto prizeY = game.prize.y + extraPrize;
//   size_t cheapest = std::numeric_limits<size_t>::max();

//   const size_t maxNumA = std::min(((prizeX / game.deltaA.x) + 1), ((prizeY / game.deltaA.y) +
//   1));
//   // fmt::println("maxNumA: {}", maxNumA);
//   for (size_t a = 0; a < maxNumA; ++a) {
//     const size_t sumXA = a * game.deltaA.x;
//     const size_t sumYA = a * game.deltaA.y;
//     const size_t costAA = a * costA;

//     const size_t deltaX = prizeX - sumXA;
//     const size_t deltaY = prizeY - sumYA;
//     if (deltaX < prizeX && deltaY < prizeY) {
//       const size_t maxNumB =
//           std::max(((deltaX / game.deltaB.x) + 1), ((deltaY / game.deltaB.y) + 1));
//       // fmt::println("a: {}, maxNumB: {}", a, maxNumB);
//       for (size_t b = 0; b < maxNumB; ++b) {
//         const size_t sumX = sumXA + (b * game.deltaB.x);
//         const size_t sumY = sumYA + (b * game.deltaB.y);
//         const size_t cost = costAA + (b * costB);

//         // fmt::println("a: {}, b: {}, sumX: {}, prizeX: {}, sumY: {}, prizeY: {}, cost: {}", a,
//         b,
//         // sumX, prizeX, sumY, prizeY, cost);
//         if ((sumX == prizeX) && (sumY == prizeY)) {
//           cheapest = std::min(cheapest, cost);
//         }
//       }
//     }
//   }

//   return (cheapest == std::numeric_limits<size_t>::max()) ? 0 : cheapest;
// }

size_t cheapestCost(const Game& game,
                    const size_t extraPrize = 0,
                    const ssize_t costA = 3,
                    const ssize_t costB = 1,
                    const size_t errorReturn = 0) {
  const ssize_t aX = static_cast<ssize_t>(game.deltaA.x);
  const ssize_t bX = static_cast<ssize_t>(game.deltaB.x);
  const ssize_t aY = static_cast<ssize_t>(game.deltaA.y);
  const ssize_t bY = static_cast<ssize_t>(game.deltaB.y);
  const ssize_t pX = static_cast<ssize_t>(game.prize.x + extraPrize);
  const ssize_t pY = static_cast<ssize_t>(game.prize.y + extraPrize);

  const ssize_t den = (aX * bY) - (bX * aY);
  if (den == 0) {
    return errorReturn;
  }

  const ssize_t a = ((bY * pX) - (bX * pY)) / den;
  const ssize_t b = ((aX * pY) - (aY * pX)) / den;

  if ((a >= 0) && (b >= 0) && ((aX * a) + (bX * b) == pX) && ((aY * a) + (bY * b) == pY)) {
    return static_cast<size_t>((costA * a) + (costB * b));
  }

  return errorReturn;
}

size_t cheapest(const std::string& path, size_t extraPrize = 0) {
  const auto games = parse(path);
  std::vector<size_t> costs{};
  std::transform(games.begin(), games.end(), std::back_inserter(costs),
                 [counter{0ULL}, &extraPrize, &games](const auto& game) mutable {
                   (void)counter;
                   (void)games;
                   // fmt::println("game {} / {}", ++counter, games.size());

                   return cheapestCost(game, extraPrize);
                 });

  // fmt::println("costs: {}", costs);
  return std::accumulate(costs.begin(), costs.end(), 0UL);
}

size_t part1(const std::string& path) {
  return cheapest(path);
}

size_t part2(const std::string& path) {
  return cheapest(path, 10000000000000);
}

int main() {
  run(1, part1, true, 480UL);
  run(1, part1, false, 35082UL);
  // run(2, part2, true, 875318608908UL);
  run(2, part2, false, 82570698600470UL);
}
