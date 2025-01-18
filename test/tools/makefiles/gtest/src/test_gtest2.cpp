#include "gtest/gtest.h"

#include <string>

TEST(GtestTest, SecondFile) {
  EXPECT_STRNE("hello", "world");
}
