#include "lib/to.h"

#include <gtest/gtest.h>

TEST(ToTest, toArithmeticfromString) {
  constexpr auto test = []<typename T>(const std::string& from, const T& expected) {
    EXPECT_EQ(to<T>(from), expected);
  };

  test("123", 123);
  test("123", 123U);
  test("123", 123UL);
  test("123", 123ULL);
  test("123", 123L);
  test("123", 123LL);
  test("123", 123.F);
  test("123", 123.);
  test("-123", -123);
  test("-123", -123L);
  test("-123", -123LL);
  test("0", 0);
  test("1.23", 1.23);
  test("-12.3", -12.3);
}

TEST(ToTest, toArithmeticVectorfromStringVector) {
  constexpr auto test = []<typename T>(const std::vector<std::string>& from,
                                       const std::vector<T>& expected) {
    EXPECT_EQ(to<std::vector<T>>(from), expected);
  };

  test({"-1", "2", "3"}, std::vector<int8_t>{-1, 2, 3});
  test({"-1", "2", "3"}, std::vector<int16_t>{-1, 2, 3});
  test({"-1", "2", "3"}, std::vector<int32_t>{-1, 2, 3});
  test({"-1", "2", "3"}, std::vector<int64_t>{-1, 2, 3});

  test({"1", "2", "3"}, std::vector<uint8_t>{1, 2, 3});
  test({"1", "2", "3"}, std::vector<uint16_t>{1, 2, 3});
  test({"1", "2", "3"}, std::vector<uint32_t>{1, 2, 3});
  test({"1", "2", "3"}, std::vector<uint64_t>{1, 2, 3});

  test({"1", "2", "3"}, std::vector<size_t>{1, 2, 3});
  test({"1 ", "2 ", "3 "}, std::vector<size_t>{1, 2, 3});
  test({" 1", " 2", " 3"}, std::vector<size_t>{1, 2, 3});
  test({" 1 ", " 2 ", " 3 "}, std::vector<size_t>{1, 2, 3});

  test({"-1", "-2", "-3"}, std::vector<size_t>{static_cast<size_t>(-1), static_cast<size_t>(-2),
                                               static_cast<size_t>(-3)});

  test({"1", "2", "3"}, std::vector<float>{1., 2., 3.});
  test({"1", "2", "3"}, std::vector<double>{1., 2., 3.});
  test({"1.5", "-2.5", "3.5"}, std::vector<float>{1.5, -2.5, 3.5});
  test({"1.5", "-2.5", "3.5"}, std::vector<double>{1.5, -2.5, 3.5});
}

TEST(ToTest, toPairfromContainer) {
  constexpr auto test = []<typename T>(const std::vector<T>& from,
                                       const std::pair<T, T>& expected) {
    EXPECT_EQ((to<std::pair<T, T>>(from)), expected);
  };

  test({"1", "2"}, std::pair<std::string, std::string>{"1", "2"});
  test({"2", "1"}, std::pair<std::string, std::string>{"2", "1"});
  test({1, 2}, std::pair<int8_t, int8_t>{1, 2});
  test({-1, -2}, std::pair<int16_t, int16_t>{-1, -2});
  test({-1, -2}, std::pair<int, int>{-1, -2});
  test({1, 2}, std::pair<size_t, size_t>{1, 2});
  test({1, 2}, std::pair<ssize_t, ssize_t>{1, 2});
  test({1., 2.}, std::pair<float, float>{1., 2.});
  test({1., 2.}, std::pair<double, double>{1., 2.});
}

TEST(ToTest, toPairArithmeticfromContainerString) {
  constexpr auto test = []<typename T>(const std::vector<std::string>& from,
                                       const std::pair<T, T>& expected) {
    EXPECT_EQ((to<std::pair<T, T>>(from)), expected);
  };

  test({"1", "2"}, std::pair<size_t, size_t>{1, 2});
  test({"2", "1"}, std::pair<size_t, size_t>{2, 1});
  test({"1.5", "2.5"}, std::pair<float, float>{1.5, 2.5});
}

TEST(ToTest, toContainerArithmeticfromString) {
  constexpr auto test = []<typename T>(const std::string& from, const std::vector<T>& expected) {
    EXPECT_EQ((to<std::vector<T>>(from)), expected);
  };

  test("123", std::vector<size_t>{1, 2, 3});
}
