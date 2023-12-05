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

std::vector<std::string> parseFile(bool example = false) {
  const auto raw = readFile(example ? "example.txt" : "input.txt");
  const auto lines = split(raw, "\n");

  return lines;
}

struct Directory {
  std::string name;
  std::vector<Directory> dirs{};
  std::map<std::string, size_t> files{};

  void print(std::ostream& os, size_t level = 0, size_t indent = 2) const {
    os << std::string(level * indent, ' ') << "- " << name << " (dir)\n";
    for (const auto& d : dirs) {
      d.print(os, level + 1);
    }

    for (const auto& f : files) {
      os << std::string((level + 1) * indent, ' ') << "- " << f.first << " (file, size=" << f.second
         << ")\n";
    }
  }

  // if too slow, memoize
  size_t size() const {
    const auto fileSize =
        std::accumulate(files.begin(), files.end(), 0UL, [](const auto& sum, const auto& file) {
          const auto& [_name, size] = file;
          return sum + size;
        });

    return std::accumulate(dirs.begin(), dirs.end(), fileSize,
                           [](const auto sum, const auto& dir) { return sum + dir.size(); });
  }

  size_t part1(size_t maxSize = 100000) const {
    size_t partialSum = size();
    if (partialSum > maxSize) {
      partialSum = 0;
    }

    return std::accumulate(dirs.begin(), dirs.end(), partialSum,
                           [](const auto sum, const auto& dir) { return sum + dir.part1(); });
  }

  void _part2(size_t required, size_t& closest) const {
    const size_t _size = size();
    if ((_size >= required) && ((_size - required) < (closest - required))) {
      closest = _size;
    }

    for (const auto& d : dirs) {
      d._part2(required, closest);
    }
  }

  size_t part2(size_t required) const {
    size_t closest = std::numeric_limits<size_t>::max();
    _part2(required, closest);
    return closest;
  }
};

std::ostream& operator<<(std::ostream& os, const Directory& dir) {
  std::stringstream s{};
  dir.print(s);
  return os << s.str();
}

Directory buildFilesystem(bool example) {
  const auto terminal = parseFile(example);

  assert(terminal[0] == "$ cd /");
  Directory root{"/"};

  std::vector<std::reference_wrapper<Directory>> stack{{root}};
  size_t line = 1;
  while (line < terminal.size()) {
    std::reference_wrapper<Directory>& cur = stack.back();
    if (terminal[line].rfind("$ cd", 0) == 0) {
      const auto parts = split(terminal[line]);
      assert(parts.size() == 3);  // $ cd <name>
      const auto& name = parts[2];
      if (name == "..") {
        assert(stack.size() > 1);
        stack.pop_back();
      } else {
        auto& dirs = cur.get().dirs;
        const auto it = std::find_if(dirs.begin(), dirs.end(),
                                     [&name](const auto& dir) { return dir.name == name; });

        if (it != dirs.end()) {
          stack.emplace_back(std::reference_wrapper<Directory>{*it});
        } else {
          throw std::runtime_error("Directory '" + name + "'not found");
        }
      }
      ++line;
    } else if (terminal[line].rfind("$ ls", 0) == 0) {
      ++line;
      while ((line < terminal.size()) && (terminal[line].rfind("$ ", 0) == std::string::npos)) {
        const auto properties = split(terminal[line]);
        assert(properties.size() == 2);
        const auto& name = properties[1];
        if (properties[0] == "dir") {
          auto& dirs = cur.get().dirs;
          const auto it = std::find_if(dirs.begin(), dirs.end(),
                                       [&name](const auto& dir) { return dir.name == name; });

          if (it == dirs.end()) {
            dirs.emplace_back(Directory{name});
          } else {
            std::cerr << "Already seen dir '" << name << "'";
          }
        } else {  // file
          auto& files = cur.get().files;
          const auto size = std::stoull(properties[0]);
          if (const auto it = files.find(name); it == files.end()) {
            files.emplace(name, size);
          } else {
            std::cerr << "Already seen file '" << name << "'";
          }
        }
        ++line;
      }
    } else {
      assert(false);
    }
  }

  return root;
}

size_t part1(bool example) {
  const auto root = buildFilesystem(example);
  // std::cout << root << std::endl;
  return root.part1();
}

size_t part2(bool example) {
  const auto root = buildFilesystem(example);
  // std::cout << root << std::endl;
  const auto available = 70000000 - root.size();
  assert(available < 30000000);
  const auto required = 30000000 - available;
  return root.part2(required);
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

  run(1, part1, true, 95437UL);
  run(1, part1, false, 1543140UL);
  run(2, part2, true, 24933642UL);
  run(2, part2, false, 1117448UL);
}
