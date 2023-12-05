#include "lib/parse.h"

#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

TEST(ParseTest, split) {
  constexpr auto test = [](std::string&& original, const std::vector<std::string>& expected,
                           const std::string& delim = " ") {
    EXPECT_EQ(split(std::move(original), delim), expected);
  };

  test("hello world", {"hello", "world"});
}

TEST(ParseTest, splitDefaultArgs) {
  EXPECT_EQ(split("hello world"), split("hello world", " "));
}

TEST(ParseTest, splitToPair) {
  EXPECT_EQ((splitTo<std::pair<std::string, std::string>>("hello world")),
            (std::pair<std::string, std::string>{"hello", "world"}));

  EXPECT_EQ((splitTo<std::pair<std::string, std::string>>("hello world")),
            (splitToPair("hello world")));
}
