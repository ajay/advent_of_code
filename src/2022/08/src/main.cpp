#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "common/parse.h"

////////////////////////////////////////////////////////////////////////////////
// printing

namespace std {

std::string to_string(const std::string& s) {
  return s;
}

}  // namespace std

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v);

template <class P1, class P2>
std::ostream& operator<<(std::ostream& os, const std::pair<P1, P2>& p);

template <size_t n, typename... T>
typename std::enable_if<(n >= sizeof...(T))>::type print_tuple(
    std::ostream&,
    const std::tuple<T...>&) {}

template <size_t n, typename... T>
typename std::enable_if<(n < sizeof...(T))>::type print_tuple(
    std::ostream& os,
    const std::tuple<T...>& t) {
  if (n != 0) {
    os << ", ";
  }

  os << std::get<n>(t);
  print_tuple<n + 1>(os, t);
}

template <typename... T>
std::ostream& operator<<(std::ostream& os, const std::tuple<T...>& t) {
  std::stringstream s{};
  s << '[';
  print_tuple<0>(s, t);
  s << ']';
  return os << s.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& o) {
  if (o) {
    os << *o;
  } else {
    os << "std::nullopt";
  }

  return os;
}

template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
  std::stringstream s{};

  s << "{\n";
  for (const auto& [key, value] : m) {
    s << "  { " << key << ", " << value << " },\n";
  }
  s << "}\n";

  return os << s.str();
}

template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
  std::stringstream s{};

  s << "{\n";
  for (const auto& [key, value] : m) {
    s << "  { " << key << ", " << value << " },\n";
  }
  s << "}\n";

  return os << s.str();
}

template <class P1, class P2>
std::ostream& operator<<(std::ostream& os, const std::pair<P1, P2>& p) {
  const auto& [one, two] = p;

  std::stringstream s{};
  s << "{ " << one << ", " << two << " }\n";

  return os << s.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v) {
  constexpr auto delim = is_specialization<T, std::set>::value ? '\n' : ' ';
  std::stringstream s{};

  s << '{' << delim;
  for (const auto& e : v) {
    s << e << delim;
  }
  s << '}';

  return os << s.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  constexpr auto delim = is_specialization<T, std::vector>::value ? '\n' : ' ';
  std::stringstream s{};

  s << '{' << delim;
  for (const auto& e : v) {
    s << e << delim;
  }
  s << '}';

  return os << s.str();
}

////////////////////////////////////////////////////////////////////////////////
// file IO / string parsing

std::string readFile(const std::string& filename) {
  std::ifstream file{filename};
  return {(std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>()};
}

////////////////////////////////////////////////////////////////////////////////
// aoc

std::vector<std::vector<size_t>> parseFile(bool example = false) {
  const auto raw = readFile(example ? "example.txt" : "input.txt");
  const auto lines = split(raw, "\n");

  std::vector<std::vector<size_t>> grid{};
  std::transform(
      lines.begin(), lines.end(), std::back_inserter(grid),
      [](const auto& line) {
        std::vector<size_t> row{};
        std::transform(
            line.begin(), line.end(), std::back_inserter(row),
            [](const auto& digit) { return std::stoull(std::string{digit}); });
        return row;
      });

  assert(grid.size());
  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
  }

  return grid;
}

bool visible(const std::vector<std::vector<size_t>>& grid,
             size_t row,
             size_t col) {
  const std::vector<std::pair<size_t, size_t>> increments{
      {-1, 0},  // decrement row, up
      {1, 0},   // increment row, down
      {0, -1},  // decrement col, left
      {0, 1},   // increment col, right
  };

  const auto sizeRow = grid.size();
  const auto sizeCol = grid[0].size();  // assume all are same

  for (const auto& [incRow, incCol] : increments) {
    auto curRow = row + incRow;
    auto curCol = col + incCol;

    while ((curRow < sizeRow) && (curCol < sizeCol) &&
           (grid[curRow][curCol] < grid[row][col])) {
      curRow += incRow;
      curCol += incCol;
    }

    if (!((curRow < sizeRow) && (curCol < sizeCol))) {
      return true;
    }
  }

  return false;
}

size_t countVisible(const std::vector<std::vector<size_t>>& grid) {
  size_t count{};
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      count += visible(grid, r, c);
    }
  }

  return count;
}

size_t score(const std::vector<std::vector<size_t>>& grid,
             size_t row,
             size_t col) {
  const std::vector<std::pair<size_t, size_t>> increments{
      {-1, 0},  // decrement row, up
      {1, 0},   // increment row, down
      {0, -1},  // decrement col, left
      {0, 1},   // increment col, right
  };

  const auto sizeRow = grid.size();
  const auto sizeCol = grid[0].size();  // assume all are same

  size_t score = 1;
  for (const auto& [incRow, incCol] : increments) {
    size_t curScore = 0;
    auto curRow = row + incRow;
    auto curCol = col + incCol;

    while ((curRow < sizeRow) && (curCol < sizeCol)) {
      curScore += 1;
      if (grid[curRow][curCol] >= grid[row][col]) {
        break;
      }

      curRow += incRow;
      curCol += incCol;
    }

    score *= curScore;
  }

  return score;
}

size_t maxScore(const std::vector<std::vector<size_t>>& grid) {
  size_t max{};
  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      max = std::max(max, score(grid, r, c));
    }
  }

  return max;
}

size_t part1(bool example) {
  const auto grid = parseFile(example);
  // std::cout << "grid: " << grid << std::endl;
  return countVisible(grid);
}

size_t part2(bool example) {
  const auto grid = parseFile(example);
  // std::cout << "grid: " << grid << std::endl;
  return maxScore(grid);
}

int main() {
  const auto run{
      [](size_t part, const auto& fn, bool example, const auto& expected) {
        const auto result{fn(example)};

        const auto str{"part" + std::to_string(part) +
                       (example ? " example: " : " input:   ") +
                       std::to_string(result) + '\n'};
        std::cout << str;

        if (result != expected) {
          const auto errStr{"result (" + std::to_string(result) +
                            ") != expected (" + std::to_string(expected) + ")"};
          throw std::runtime_error(errStr);
        }
      }};

  run(1, part1, true, 21UL);
  run(1, part1, false, 1805UL);
  run(2, part2, true, 8UL);
  run(2, part2, false, 444528UL);
}
