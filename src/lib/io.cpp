#include "lib/io.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <boost/algorithm/string/trim.hpp>

std::string read(const std::string& path, bool trim) {
  assert(std::filesystem::exists(path));
  std::ifstream file{path};

  std::string data = {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

  if (trim) {
    boost::trim(data);
  }

  return data;
}
