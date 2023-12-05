#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// printing

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v);

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

template <class T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& p) {
  const auto& [one, two] = p;

  std::stringstream s{};
  s << "{ " << one << ", " << two << " }\n";

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

std::string readFile(const std::string& filename = "input.txt") {
  std::ifstream file{filename};
  return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
  std::vector<std::string> v{};
  size_t start = 0;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    v.push_back(str.substr(start, end - start));
  }

  return v;
}

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec) {
  std::vector<size_t> numVec{};

  std::transform(strVec.begin(), strVec.end(), std::back_inserter(numVec),
                 [](const std::string& s) { return std::stoul(s); });

  return numVec;
}

std::pair<size_t, size_t> strVecToPair(const std::vector<std::string>& strVec) {
  const auto v = strVecToNumVec(strVec);
  assert(v.size() == 2);
  return {v[0], v[1]};
}

std::vector<size_t> strToNumVec(const std::string& str) {
  const std::vector<char> digits{str.begin(), str.end()};

  std::vector<std::string> digitsStr{};
  std::transform(digits.begin(), digits.end(), std::back_inserter(digitsStr),
                 [](const char& c) { return std::string(1, c); });

  return strVecToNumVec(digitsStr);
}

////////////////////////////////////////////////////////////////////////////////
// aoc

std::vector<std::vector<size_t>> parseFile() {
  const auto raw = readFile();
  const auto lines = split(raw, "\n");

  std::vector<std::vector<size_t>> data{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(data),
                 [](const auto& line) { return strToNumVec(line); });

  return data;
}

size_t part1(size_t steps = 100, bool returnAllFlashed = false) {
  auto grid = parseFile();

  const auto increment = [&grid](size_t r, size_t c,
                                 const std::vector<std::vector<bool>>& flashed) {
    if ((r < grid.size()) && (c < grid[r].size())) {
      if (!flashed[r][c]) {
        ++grid[r][c];
      }
    }
  };

  const auto incrementAll = [&grid]() {
    for (size_t r = 0; r < grid.size(); ++r) {
      for (size_t c = 0; c < grid[r].size(); ++c) {
        ++grid[r][c];
      }
    }
  };

  const auto flash = [&grid, &increment](size_t r, size_t c,
                                         std::vector<std::vector<bool>>& flashed) {
    flashed[r][c] = true;
    grid[r][c] = 0;
    increment(r - 1, c, flashed);
    increment(r + 1, c, flashed);
    increment(r, c - 1, flashed);
    increment(r, c + 1, flashed);
    increment(r - 1, c - 1, flashed);
    increment(r + 1, c + 1, flashed);
    increment(r - 1, c + 1, flashed);
    increment(r + 1, c - 1, flashed);
  };

  const auto countFlashes = [](const std::vector<std::vector<bool>>& flashed) {
    // std::cout << flashed << '\n';

    return std::accumulate(
        flashed.begin(), flashed.end(), 0UL, [](const auto& sum, const auto& row) {
          return sum +
                 std::accumulate(row.begin(), row.end(), 0UL,
                                 [](const auto& sumRow, const auto& f) { return sumRow + f; });
        });
  };

  const auto step = [&grid, &flash, &countFlashes]() {
    std::vector<std::vector<bool>> flashed(grid.size());
    for (size_t r = 0; r < grid.size(); ++r) {
      flashed[r].resize(grid[r].size());
    }

    size_t flashes = 0;
    bool keepGoing = false;

    do {
      keepGoing = false;
      for (size_t r = 0; r < grid.size(); ++r) {
        for (size_t c = 0; c < grid[r].size(); ++c) {
          if (grid[r][c] > 9) {
            flash(r, c, flashed);
          }
        }
      }
      if (countFlashes(flashed) > flashes) {
        keepGoing = true;
        flashes = countFlashes(flashed);
      }

    } while (keepGoing);

    return countFlashes(flashed);
  };

  const auto allFlashed = [&grid]() {
    return std::all_of(grid.begin(), grid.end(), [](const auto& row) {
      return std::all_of(row.begin(), row.end(), [](const auto& e) { return e == 0; });
    });
  };

  size_t totalFlashes = 0;
  for (size_t s = 1; s <= steps; ++s) {
    incrementAll();
    totalFlashes += step();

    if (returnAllFlashed && allFlashed()) {
      return s;
    }
  }

  return totalFlashes;
}

size_t part2() {
  return part1(100000, true);
}

int main() {
  const auto p1 = part1();
  assert(p1 == 1755);
  std::cout << "part1: " << p1 << '\n';

  const auto p2 = part2();
  assert(p2 == 212);
  std::cout << "part2: " << p2 << '\n';
}
