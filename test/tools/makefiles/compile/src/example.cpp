#include <cstddef>
#include <iostream>

#include <boost/lexical_cast.hpp>
#if defined __has_include && __has_include("fmt/base.h")
#include <fmt/base.h>
#endif
#include <fmt/core.h>  // IWYU pragma: keep

int main() {
  fmt::print("Hello {}!\n", "world");
  std::cout << "Meaning of life? " << boost::lexical_cast<size_t>("42") << std::endl;
}
