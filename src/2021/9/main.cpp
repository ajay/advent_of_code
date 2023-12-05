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
#include <unordered_map>
#include <vector>

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
std::ostream& operator<<(std::ostream& os, const std::set<T>& v) {
  constexpr auto delim = ' ';
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
  constexpr auto delim = ' ';
  std::stringstream s{};

  s << '{' << delim;
  for (const auto& e : v) {
    s << e << delim;
  }
  s << '}';

  return os << s.str();
}

std::vector<std::vector<size_t>> parseFile() {
  const auto raw = readFile();
  const auto lines = split(raw, "\n");

  std::vector<std::vector<size_t>> grid{};
  for (const auto& line : lines) {
    const std::vector<char> digits{line.begin(), line.end()};

    std::vector<std::string> digitsStr{};
    std::transform(digits.begin(), digits.end(), std::back_inserter(digitsStr),
                   [](const char& c) { return std::string(1, c); });

    grid.emplace_back(strVecToNumVec(digitsStr));
  }

  return grid;
}

std::vector<std::pair<size_t, size_t>> getLowPoints(const std::vector<std::vector<size_t>>& data) {
  std::vector<std::pair<size_t, size_t>> points{};

  for (size_t r = 0; r < data.size(); ++r) {
    for (size_t c = 0; c < data[r].size(); ++c) {
      bool lowest = true;

      if ((r > 0) && (data[r][c] >= data[r - 1][c])) {
        lowest = false;
      }

      if (((r + 1) < data.size()) && (data[r][c] >= data[r + 1][c])) {
        lowest = false;
      }

      if ((c > 0) && (data[r][c] >= data[r][c - 1])) {
        lowest = false;
      }

      if (((c + 1) < data[r].size()) && (data[r][c] >= data[r][c + 1])) {
        lowest = false;
      }

      if (lowest) {
        points.emplace_back(std::pair<size_t, size_t>{r, c});
      }
    }
  }

  return points;
}

size_t part1() {
  const auto data = parseFile();
  const auto lowPoints = getLowPoints(data);

  return std::accumulate(lowPoints.begin(), lowPoints.end(), 0UL,
                         [&data](const auto& sum, const auto& point) {
                           const auto& [r, c] = point;
                           return sum + data[r][c] + 1;
                         });
}

size_t part2() {
  const auto data = parseFile();
  const auto lowPoints = getLowPoints(data);

  std::vector<size_t> sizes{};

  for (const auto& point : lowPoints) {
    size_t size = 0;
    auto neighbors = decltype(lowPoints){point};
    std::set<std::pair<size_t, size_t>> seen{};

    while (neighbors.size()) {
      const auto neighbor = neighbors.back();
      neighbors.pop_back();

      const auto inBasin = [&data](const auto& p) {
        const auto& [r, c] = p;
        return ((r >= 0) && (r < data.size()) && (c >= 0) && (c < data[r].size()) &&
                (data[r][c] != 9));
      };

      if ((std::count(seen.begin(), seen.end(), neighbor) == 0) && inBasin(neighbor)) {
        ++size;
        seen.emplace(neighbor);
        const auto& [r, c] = neighbor;
        neighbors.emplace_back(std::pair<size_t, size_t>{r + 1, c});
        neighbors.emplace_back(std::pair<size_t, size_t>{r - 1, c});
        neighbors.emplace_back(std::pair<size_t, size_t>{r, c + 1});
        neighbors.emplace_back(std::pair<size_t, size_t>{r, c - 1});
      }
    }

    sizes.emplace_back(size);
  }

  std::sort(sizes.begin(), sizes.end());
  const auto s = sizes.size();

  return sizes[s - 1] * sizes[s - 2] * sizes[s - 3];
}

int main() {
  const auto p1 = part1();
  assert(p1 == 577);
  std::cout << "part1: " << p1 << '\n';

  const auto p2 = part2();
  assert(p2 == 1069200);
  std::cout << "part2: " << p2 << '\n';
}
