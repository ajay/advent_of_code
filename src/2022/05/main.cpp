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
typename std::enable_if<(n >= sizeof...(T))>::type print_tuple(std::ostream&,
                                                               const std::tuple<T...>&) {}

template <size_t n, typename... T>
typename std::enable_if<(n < sizeof...(T))>::type print_tuple(std::ostream& os,
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

std::pair<std::vector<std::vector<char>>, std::vector<std::tuple<size_t, size_t, size_t>>>
parseFile(bool example = false) {
  const auto raw = readFile(example ? "example.txt" : "input.txt");
  const auto sections = split(raw, "\n\n");
  assert(sections.size() == 2);

  const auto stackLines = split(sections[0], "\n");
  const auto& stackBottom =
      *std::next(stackLines.rbegin());  // bottom of stacks, assume each stack has at least 1
  const auto numStacks = static_cast<size_t>(std::count_if(
      stackBottom.begin(), stackBottom.end(), [](const auto& c) { return std::isalpha(c); }));
  std::vector<std::vector<char>> stacks(numStacks);

  // skip last line (stack indexes), second last to first
  std::for_each(
      std::next(stackLines.rbegin()), stackLines.rend(), [&stacks, &numStacks](const auto& line) {
        for (size_t i = 0; i < numStacks; ++i) {
          if (const auto idx{(i * 4) + 1}; (idx < line.size()) && std::isalpha(line[idx])) {
            stacks[i].emplace_back(line[idx]);
          }
        }
      });

  const auto procedureLines = split(sections[1], "\n");
  std::vector<std::tuple<size_t, size_t, size_t>> procedure{};
  std::transform(procedureLines.begin(), procedureLines.end(), std::back_inserter(procedure),
                 [](const auto& line) {
                   const auto splitLine = split(line);
                   assert(splitLine.size() == 6);
                   const auto num = std::stoull(splitLine[1]);
                   const auto from = std::stoull(splitLine[3]);
                   const auto to = std::stoull(splitLine[5]);
                   return std::tuple<size_t, size_t, size_t>{num, from, to};
                 });

  return {stacks, procedure};
}

std::string topOfStacks(bool example, bool moveTogether) {
  auto [stacks, procedure] = parseFile(example);

  for (const auto& [num, _from, _to] : procedure) {
    const auto from = _from - 1;
    const auto to = _to - 1;

    assert(to != from);
    assert(num <= stacks[from].size());

    if (moveTogether) {
      const auto begin = std::prev(stacks[from].end(), static_cast<ssize_t>(num));
      const auto end = stacks[from].end();
      std::copy(begin, end, std::back_inserter(stacks[to]));
      stacks[from].erase(begin, end);
    } else {
      const auto begin = stacks[from].rbegin();
      const auto end = std::next(stacks[from].rbegin(), static_cast<ssize_t>(num));
      std::copy(begin, end, std::back_inserter(stacks[to]));
      stacks[from].erase(end.base(), begin.base());
    }
  }

  std::string top{};
  std::transform(stacks.begin(), stacks.end(), std::back_inserter(top),
                 [](const auto& stack) { return stack.back(); });

  return top;
}

std::string part1(bool example) {
  return topOfStacks(example, false);
}

std::string part2(bool example) {
  return topOfStacks(example, true);
}

int main() {
  const auto run{[](size_t part, const auto& fn, bool example, const auto& expected) {
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

  run(1, part1, true, std::string{"CMZ"});
  run(1, part1, false, std::string{"GRTSWNJHH"});
  run(2, part2, true, std::string{"MCD"});
  run(2, part2, false, std::string{"QLFQDBBHM"});
}
