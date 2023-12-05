#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
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

////////////////////////////////////////////////////////////////////////////////
// printing

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

std::string readFile(const std::string& filename = "input.txt") {
  std::ifstream file{filename};
  return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
  std::vector<std::string> v{};
  size_t begin = 0;
  size_t end = 0;

  while ((begin = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, begin);
    v.push_back(str.substr(begin, end - begin));
  }

  return v;
}

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec) {
  std::vector<size_t> numVec{};

  std::transform(strVec.begin(), strVec.end(), std::back_inserter(numVec),
                 [](const std::string& s) { return std::stoul(s); });

  return numVec;
}

std::pair<std::string, std::string> strVecToStrPair(const std::vector<std::string>& strVec) {
  assert(strVec.size() == 2);
  return {strVec[0], strVec[1]};
}

std::pair<size_t, size_t> strVecToNumPair(const std::vector<std::string>& strVec) {
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

using Point = std::pair<size_t, size_t>;

// based off https://en.wikipedia.org/wiki/A*_search_algorithm
std::optional<std::vector<Point>> aStar(
    const Point& start,
    const Point& finish,
    const std::function<size_t(const Point&, const Point&)>& heuristic,
    const std::function<size_t(const Point&, const Point&)>& weight,
    const std::function<std::vector<Point>(const Point&)>& neighbors) {
  const auto reconstructPath = [](const std::map<Point, Point>& cameFrom, const Point& current) {
    std::vector<Point> path{current};

    auto it = cameFrom.find(current);
    while (it != cameFrom.end()) {
      const auto& [curr, from] = *it;
      path.emplace_back(from);
      it = cameFrom.find(from);
    }

    std::reverse(path.begin(), path.end());

    return path;
  };

  std::set<Point> openSet{start};
  std::map<Point, Point> cameFrom{};

  std::map<Point, size_t> gScore{
      {start, 0},
  };

  std::map<Point, size_t> fScore{
      {start, heuristic(start, finish)},
  };

  const auto getGScore = [&gScore](const auto& point) {
    size_t score = SIZE_MAX;

    if (const auto it = gScore.find(point); it != gScore.end()) {
      score = it->second;
    }

    return score;
  };

  const auto getFScore = [&fScore](const auto& point) {
    size_t score = SIZE_MAX;

    if (const auto it = fScore.find(point); it != fScore.end()) {
      score = it->second;
    }

    return score;
  };

  while (openSet.size()) {
    const auto current = *std::min_element(
        openSet.begin(), openSet.end(),
        [&getFScore](const auto& p1, const auto& p2) { return getFScore(p1) < getFScore(p2); });

    // std::cout << "current: (" << current.first << ", " << current.second <<
    // ")\n";

    openSet.erase(current);

    if (current == finish) {
      return reconstructPath(cameFrom, current);
    }

    for (const auto& neighbor : neighbors(current)) {
      const size_t currGScore = getGScore(current) + weight(current, neighbor);

      if (currGScore < getGScore(neighbor)) {
        cameFrom[neighbor] = current;
        gScore[neighbor] = currGScore;
        fScore[neighbor] = currGScore + heuristic(neighbor, finish);

        openSet.emplace(neighbor);
      }
    }
  }

  return {};
}

std::optional<std::vector<Point>> runAStar(const std::vector<std::vector<size_t>>& grid,
                                           const Point& start,
                                           const Point& finish) {
  const auto heuristic = [](const Point& current, const Point& goal) {
    // manhattan distance
    constexpr size_t movementCost = 1;

    const auto& [x1, y1] = current;
    const auto& [x2, y2] = goal;
    const size_t dx =
        static_cast<size_t>(std::abs(static_cast<ssize_t>(x1) - static_cast<ssize_t>(x2)));
    const size_t dy =
        static_cast<size_t>(std::abs(static_cast<ssize_t>(y1) - static_cast<ssize_t>(y2)));
    return movementCost * (dx + dy);
  };

  const auto weight = [&grid](const Point& from [[gnu::unused]], const Point& to) {
    const auto& [x, y] = to;
    return grid[x][y];
  };

  const auto neighbors = [&grid](const Point& point) {
    std::vector<Point> n{};

    const auto& [x, y] = point;

    if (x) {
      n.push_back({(x - 1), y});
    }

    if (y) {
      n.push_back({x, y - 1});
    }

    if (x < (grid.size() - 1)) {
      n.push_back({x + 1, y});
    }

    if (y < (grid[0].size() - 1)) {
      n.push_back({x, y + 1});
    }

    return n;
  };

  return aStar(start, finish, heuristic, weight, neighbors);
}

std::vector<std::vector<size_t>> parseFile() {
  const auto raw = readFile();
  const auto lines = split(raw, "\n");

  std::vector<std::vector<size_t>> grid{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(grid),
                 [](const auto& line) { return strToNumVec(line); });

  return grid;
}

size_t part1(const std::vector<std::vector<size_t>>& grid = parseFile()) {
  // std::cerr << "grid: " << grid << std::endl << std::endl;

  const auto path = runAStar(grid, {0, 0}, {(grid.size() - 1), (grid[0].size() - 1)});

  // if (path) {
  //     std::cout << "path: \n" << *path << std::endl << std::endl;
  // } else {
  //     std::cout << "no path found!!\n" << std::endl;
  // }

  size_t risk = 0;
  if (path && (path->size() > 1)) {
    risk = std::accumulate(std::next(path->begin()), path->end(), 0UL,
                           [&grid](const auto& sum, const auto& point) {
                             const auto& [x, y] = point;
                             return sum + grid[x][y];
                           });
  }

  return risk;
}

size_t part2(size_t multiplier = 5) {
  const auto data = parseFile();

  std::vector<std::vector<size_t>> grid(data.size() * multiplier);
  for (auto& row : grid) {
    row.resize(data[0].size() * multiplier);
  }

  for (size_t x = 0; x < grid.size(); ++x) {
    for (size_t y = 0; y < grid[x].size(); ++y) {
      const size_t dataX = x % data.size();
      const size_t dataY = y % data[dataX].size();

      const size_t xIncrease = x / data.size();
      const size_t yIncrease = y / data[dataX].size();

      size_t total = data[dataX][dataY] + xIncrease + yIncrease;

      while (total > 9) {
        total -= 9;
      }

      grid[x][y] = total;
    }
  }

  return part1(grid);
}

int main() {
  const auto p1 = part1();
  assert(p1 == 755);
  std::cout << "part1: " << p1 << '\n';

  const auto p2 = part2();
  assert(p2 == 3016);
  std::cout << "part2: " << p2 << '\n';
}
