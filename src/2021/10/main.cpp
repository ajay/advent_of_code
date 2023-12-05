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

std::vector<std::vector<char>> parseFile() {
  const auto raw = readFile();
  const auto lines = split(raw, "\n");

  std::vector<std::vector<char>> data{};

  for (const auto& line : lines) {
    data.emplace_back(std::vector<char>{line.begin(), line.end()});
  }

  return data;
}

size_t part1() {
  const auto data = parseFile();

  size_t sum = 0;
  for (const auto& line : data) {
    std::vector<char> stack{};

    for (const auto& c : line) {
      if ((c == '(') || (c == '[') || (c == '{') || (c == '<')) {
        stack.emplace_back(c);
      } else if (c == ')') {
        if (stack.size() && stack.back() == '(') {
          stack.pop_back();
        } else {
          sum += 3;
          break;
        }
      } else if (c == ']') {
        if (stack.size() && stack.back() == '[') {
          stack.pop_back();
        } else {
          sum += 57;
          break;
        }
      } else if (c == '}') {
        if (stack.size() && stack.back() == '{') {
          stack.pop_back();
        } else {
          sum += 1197;
          break;
        }
      } else if (c == '>') {
        if (stack.size() && stack.back() == '<') {
          stack.pop_back();
        } else {
          sum += 25137;
          break;
        }
      }
    }
  }

  return sum;
}

size_t part2() {
  const auto data = parseFile();

  std::vector<size_t> scores{};
  for (const auto& line : data) {
    std::vector<char> stack{};

    bool error = false;
    for (const auto& c : line) {
      if ((c == '(') || (c == '[') || (c == '{') || (c == '<')) {
        stack.emplace_back(c);
      } else if (c == ')') {
        if (stack.size() && stack.back() == '(') {
          stack.pop_back();
        } else {
          error = true;
          break;
        }
      } else if (c == ']') {
        if (stack.size() && stack.back() == '[') {
          stack.pop_back();
        } else {
          error = true;
          break;
        }
      } else if (c == '}') {
        if (stack.size() && stack.back() == '{') {
          stack.pop_back();
        } else {
          error = true;
          break;
        }
      } else if (c == '>') {
        if (stack.size() && stack.back() == '<') {
          stack.pop_back();
        } else {
          error = true;
          break;
        }
      }
    }

    if (!error) {
      size_t sum = 0;

      std::for_each(stack.rbegin(), stack.rend(), [&sum](const auto& c) {
        sum *= 5;
        switch (c) {
          case '(':
            sum += 1;
            break;
          case '[':
            sum += 2;
            break;
          case '{':
            sum += 3;
            break;
          case '<':
            sum += 4;
            break;
        }
      });

      scores.emplace_back(sum);
    }
  }

  std::sort(scores.begin(), scores.end());

  return scores[scores.size() / 2];
}

int main() {
  const auto p1 = part1();
  assert(p1 == 358737);
  std::cout << "part1: " << p1 << '\n';

  const auto p2 = part2();
  assert(p2 == 4329504793);
  std::cout << "part2: " << p2 << '\n';
}
