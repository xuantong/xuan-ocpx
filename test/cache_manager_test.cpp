//
// Created by xuantong on 2024/3/25.
//
#include "cache_manager.hpp"
#include <gtest/gtest.h>
#include <iostream>

// 一个简单的加载函数，返回给定键的两倍作为值
int SimpleLoader(const int &key) {
  return key * 2;
}

TEST(CacheManagerTest, InitializationAndGet) {
  CacheManager<int, int> cache_manager(SimpleLoader, std::chrono::milliseconds(500));

  int value = cache_manager.Get(1);
  EXPECT_EQ(value, 2);
}
