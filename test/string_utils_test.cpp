//
// Created by xuantong on 2024/3/24.
//
#include <gtest/gtest.h>
#include "string_utils.hpp"

// 测试URL编码函数
TEST(string_utils_test, urlEncode) {
  std::string original = "This is a test: 1+2 = 3!";
  std::string expected = "This%20is%20a%20test%3A%201%2B2%20%3D%203%21";
  ASSERT_EQ(expected, urlEncode(original));
}

TEST(string_utils_test, randomString) {
  for (int i = 10; i < 20; ++i) {
    std::string expected = randomString(i);
    ASSERT_EQ(expected.length(), i);
  }
}

