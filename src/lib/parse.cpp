#include "parse.h"

#include <algorithm>
#include <cassert>

#include <boost/algorithm/string.hpp>

std::vector<std::string> split(std::string&& str, const std::string& delim, bool trim) {
  std::vector<std::string> v{};

  if (trim) {
    boost::trim(str);
  }

  boost::split(v, str, boost::is_any_of(delim));

  return v;
}
