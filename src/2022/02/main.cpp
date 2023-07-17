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
  return {(std::istreambuf_iterator<char>(file)),
          std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str,
                               const std::string& delim = " ") {
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

std::pair<std::string, std::string> strVecToStrPair(
    const std::vector<std::string>& strVec) {
  assert(strVec.size() == 2);
  return {strVec[0], strVec[1]};
}

std::pair<size_t, size_t> strVecToNumPair(
    const std::vector<std::string>& strVec) {
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

std::vector<std::pair<char, char>> parseFile(bool example = false) {
  const auto raw = readFile(example ? "example.txt" : "input.txt");
  const auto lines = split(raw, "\n");

  std::vector<std::pair<char, char>> turns{};
  std::transform(lines.begin(), lines.end(), std::back_inserter(turns),
                 [](const auto& line) {
                   const auto chars = split(line);
                   assert(chars.size() == 2);
                   assert(chars[0].size() == 1);
                   assert(chars[1].size() == 1);
                   return std::pair<char, char>{chars[0][0], chars[1][0]};
                 });

  return turns;
}

size_t part1(std::vector<std::pair<char, char>> turns = parseFile()) {
  const std::unordered_map<char, size_t> choices{
      {'X', 1},  // A rock
      {'Y', 2},  // B paper
      {'Z', 3},  // C scissors
  };

  size_t score{};
  for (const auto& [opponent, turn] : turns) {
    const auto choice = choices.find(turn);
    assert(choice != choices.end());
    score += choice->second;

    if ((turn - ('X' - 'A')) == opponent) {
      score += 3;
    } else if ((turn == 'X' && opponent == 'C') ||  // rock beats scissors
               (turn == 'Y' && opponent == 'A') ||  // paper beats rock
               (turn == 'Z' && opponent == 'B')) {  // scissors beat paper
      score += 6;
    }
  }

  return score;
}

size_t part2() {
  auto turns{parseFile()};
  for (auto& [opponent, result] : turns) {
    switch (result) {
      case 'Y':  // draw
        result = opponent + ('X' - 'A');
        break;
      case 'X':  // need to lose
        switch (opponent) {
          case 'A':        // rock
            result = 'Z';  // scissors
            break;
          case 'B':        // paper
            result = 'X';  // rock
            break;
          case 'C':        // scissors
            result = 'Y';  // paper
            break;
        }
        break;
      case 'Z':  // need to win
        switch (opponent) {
          case 'A':        // rock
            result = 'Y';  // paper
            break;
          case 'B':        // paper
            result = 'Z';  // scissors
            break;
          case 'C':        // scissors
            result = 'X';  // rock
            break;
        }
    }
  }

  return part1(turns);
}

int main() {
  const auto p1 = part1();
  std::cout << "part1: " << p1 << '\n';
  assert(p1 == 15632);

  const auto p2 = part2();
  std::cout << "part2: " << p2 << '\n';
  assert(p2 == 14416);
}
