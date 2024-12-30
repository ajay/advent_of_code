#include "gtest/gtest.h"

TEST(GtestTest, Trivial) {
  EXPECT_FALSE(false);
  EXPECT_TRUE(true);
}
