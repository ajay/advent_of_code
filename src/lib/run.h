#include <cstddef>
#include <source_location>
#include <string>

#include <fmt/core.h>

template <class Function, class Result>
void run_(const std::source_location& location,
          size_t part,
          const Function& fn,
          bool example,
          const Result& expected,
          const std::string& pathExample = "data/example.txt",
          const std::string& pathInput = "data/input.txt") {
  const auto result = fn(example ? pathExample : pathInput);

  fmt::print("{}({}) part {:d} {:<8} {}\n", location.file_name(), location.line(), part,
             (example ? "example:" : "input:"), result);

  if (result != expected) {
    const auto error = fmt::format("result ({}) != expected ({})", result, expected);
    throw std::runtime_error(error);
  }
}

// Macro to get around clang15 std::source_location bug, fixed in clang16+ (not
// easily available as of July 2023). See
// github.com/llvm/llvm-project/issues/56379.
#define run(...) run_(std::source_location::current(), ##__VA_ARGS__)
