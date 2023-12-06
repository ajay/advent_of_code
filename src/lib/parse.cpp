#include "parse.h"

#include <algorithm>
#include <cassert>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>

std::vector<std::string> split(std::string&& str,
                               const std::string& delim,
                               bool delimMultiChar,
                               bool trimOriginal,
                               bool trimSplit) {
  std::vector<std::string> v{};

  if (trimOriginal) {
    boost::trim(str);
  }

  if (delimMultiChar) {
    boost::algorithm::split_regex(v, str, boost::regex(delim));
  } else {
    boost::split(v, str, boost::is_any_of(delim), boost::algorithm::token_compress_on);
  }

  if (trimSplit) {
    for (auto& s : v) {
      boost::trim(s);
    }
  }

  return v;
}
