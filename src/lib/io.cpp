#include "lib/io.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

std::string read(const std::string& path, bool trim) {
  std::ifstream file{path};

  std::string data = {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

  if (trim) {
    boost::trim(data);
  }

  return data;
}
