#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
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
                 [](const auto& s) { return std::stoi(s); });

  return numVec;
}

std::pair<size_t, size_t> strVecToPair(const std::vector<std::string>& strVec) {
  const auto v = strVecToNumVec(strVec);
  assert(v.size() == 2);
  return {v[0], v[1]};
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::pair<T, T>& p) {
  const auto& [one, two] = p;

  std::stringstream s{};
  s << "{ " << one << ", " << two << " }";

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

std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> parseFile() {
  std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> points{};

  const auto raw = readFile();
  const auto lines = split(raw, "\n");

  for (const auto& line : lines) {
    const auto pairs = split(line, " -> ");
    const auto pair0 = split(pairs[0], ",");
    const auto pair1 = split(pairs[1], ",");

    points.emplace_back(strVecToPair(pair0), strVecToPair(pair1));
  }

  return points;
}

size_t pointMax(
    const std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>>& points) {
  size_t max = 0;

  for (const auto& pair : points) {
    const auto& [p1, p2] = pair;
    const auto& [x1, y1] = p1;
    const auto& [x2, y2] = p2;
    max = std::max({max, x1, y1, x2, y2});
  }

  return max;
}

std::vector<std::vector<size_t>> pointMap(size_t max) {
  std::vector<std::vector<size_t>> pm(max);

  for (auto& v : pm) {
    v.resize(max);
  }

  return pm;
}

size_t part1(bool diagonals = false) {
  const auto points = parseFile();
  auto map = pointMap(pointMax(points) + 1);

  // std::cout << "\npoints:\n" << points << '\n';
  // std::cout << "\nmax: " << max << '\n';

  for (const auto& pair : points) {
    const auto& [p1, p2] = pair;
    const auto& [x1, y1] = p1;
    const auto& [x2, y2] = p2;

    // std::cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';

    if (x1 == x2) {
      const auto yMin = std::min(y1, y2);
      const auto yMax = std::max(y1, y2);

      for (size_t y = yMin; y <= yMax; ++y) {
        ++map[x1][y];
      }
    } else if (y1 == y2) {
      const auto xMin = std::min(x1, x2);
      const auto xMax = std::max(x1, x2);

      for (size_t x = xMin; x <= xMax; ++x) {
        ++map[x][y1];
      }
    } else if (diagonals) {
      if (((x1 > x2) && (y1 > y2)) || ((x2 > x1) && (y2 > y1))) {
        const size_t startX = std::min(x1, x2);
        const size_t startY = std::min(y1, y2);
        const size_t endX = std::max(x1, x2);
        const size_t endY = std::max(y1, y2);

        for (size_t x = startX, y = startY; (x <= endX) && (y <= endY); ++x, ++y) {
          ++map[x][y];
        }
      } else {
        const size_t startX = std::min(x1, x2);
        const size_t startY = std::max(y1, y2);
        const size_t endX = std::max(x1, x2);
        const size_t endY = std::min(y1, y2);

        for (size_t x = startX, y = startY; (x <= endX) && (y >= endY); ++x, --y) {
          ++map[x][y];
        }
      }
    }
  }

  // std::cout << map;

  return std::accumulate(map.begin(), map.end(), 0UL, [](const size_t& sum, const auto& row) {
    return sum + static_cast<size_t>(
                     std::count_if(row.begin(), row.end(), [](const auto& e) { return e > 1; }));
  });
}

size_t part2() {
  return part1(true);
}

int main() {
  const auto p1 = part1();
  assert(p1 == 7644);
  std::cout << "part1: " << p1 << '\n';

  const auto p2 = part2();
  assert(p2 == 18627);
  std::cout << "part2: " << p2 << '\n';
}
