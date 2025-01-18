#include "gtest/gtest.h"

#include <string>

TEST(GtestTest, Trivial) {
  EXPECT_FALSE(false);
  EXPECT_TRUE(true);
}
