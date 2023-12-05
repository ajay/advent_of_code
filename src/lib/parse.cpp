#include "parse.h"

#include <algorithm>
#include <cassert>

#include <boost/algorithm/string.hpp>

std::vector<std::string> split(std::string&& str,
                               const std::string& delim,
                               bool trimOriginal,
                               bool trimSplit) {
  std::vector<std::string> v{};

  if (trimOriginal) {
    boost::trim(str);
  }

  boost::split(v, str, boost::is_any_of(delim));

  if (trimSplit) {
    for (auto& s : v) {
      boost::trim(s);
    }
  }

  return v;
}
