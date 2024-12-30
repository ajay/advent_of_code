#include <cstddef>
#include <iostream>

#include <fmt/core.h>
#include <boost/lexical_cast.hpp>

int main() {
  fmt::print("Hello {}!\n", "world");
  std::cout << "Meaning of life? " << boost::lexical_cast<size_t>("42") << std::endl;
}
