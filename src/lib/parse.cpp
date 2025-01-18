#include "parse.h"

#include <algorithm>
#include <map>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/detail/classification.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/core/addressof.hpp>
#include <boost/function/function_base.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/regex/v5/basic_regex.hpp>
#include <boost/regex/v5/pattern_except.hpp>
#include <boost/regex/v5/perl_matcher.hpp>
#include <boost/regex/v5/perl_matcher_common.hpp>
#include <boost/regex/v5/perl_matcher_non_recursive.hpp>
#include <boost/regex/v5/regex_fwd.hpp>
#include <boost/regex/v5/regex_search.hpp>
#include <boost/regex/v5/regex_traits.hpp>
#include <boost/regex/v5/regex_traits_defaults.hpp>

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
