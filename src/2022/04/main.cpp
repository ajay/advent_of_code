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

std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> parseFile(
    bool example = false) {
  const auto raw = readFile(example ? "example.txt" : "input.txt");
  const auto lines = split(raw, "\n");

  std::vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> pairs{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(pairs), [](const auto& line) {
    const auto pair = split(line, ",");
    assert(pair.size() == 2);

    const auto left = split(pair[0], "-");
    const auto right = split(pair[1], "-");
    assert(left.size() == 2);
    assert(right.size() == 2);

    return std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>{
        {std::stoul(left[0]), std::stoul(left[1])}, {std::stoul(right[0]), std::stoul(right[1])}};
  });

  return pairs;
}

size_t countOverlaps(bool example, bool fullOverlap) {
  const auto pairs{parseFile(example)};

  size_t count{0};
  for (const auto& [left, right] : pairs) {
    const auto& [lMin, lMax]{left};
    const auto& [rMin, rMax]{right};
    assert(lMin <= lMax);
    assert(rMin <= rMax);

    std::vector<size_t> l(lMax - lMin + 1);
    std::vector<size_t> r(rMax - rMin + 1);
    std::iota(l.begin(), l.end(), lMin);
    std::iota(r.begin(), r.end(), rMin);

    if (fullOverlap) {
      if (std::includes(l.begin(), l.end(), r.begin(), r.end()) ||
          std::includes(r.begin(), r.end(), l.begin(), l.end())) {
        ++count;
      }
    } else {
      std::vector<size_t> intersection{};
      std::set_intersection(l.begin(), l.end(), r.begin(), r.end(),
                            std::back_inserter(intersection));
      if (intersection.size()) {
        ++count;
      }
    }
  }

  return count;
}

size_t part1(bool example) {
  return countOverlaps(example, true);
}

size_t part2(bool example) {
  return countOverlaps(example, false);
}

int main() {
  const auto run{[](size_t part, const auto& fn, bool example, size_t expected) {
    const auto result{fn(example)};

    const auto str{"part" + std::to_string(part) + (example ? " example: " : " input:   ") +
                   std::to_string(result) + '\n'};
    std::cout << str;

    if (result != expected) {
      const auto errStr{"result (" + std::to_string(result) + ") != expected (" +
                        std::to_string(expected) + ")"};
      throw std::runtime_error(errStr);
    }
  }};

  run(1, part1, true, 2);
  run(1, part1, false, 498);
  run(2, part2, true, 4);
  run(2, part2, false, 859);
}
