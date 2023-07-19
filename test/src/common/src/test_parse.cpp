#include "common/parse.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

TEST(ParseTest, split) {
  constexpr auto test = [](std::string&& original,
                           const std::vector<std::string>& expected,
                           const std::string& delim = " ") {
    EXPECT_EQ(split(std::move(original), delim), expected);
  };

  test("hello world", {"hello", "world"});
}

TEST(ParseTest, splitDefaultArgs) {
  EXPECT_EQ(split(std::move("hello world")), split("hello world", " "));
}
