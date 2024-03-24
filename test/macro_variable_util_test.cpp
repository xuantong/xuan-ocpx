//
// Created by xuantong on 2024/3/23.
//

#include <gtest/gtest.h>
#include "macro_variable_util.hpp" // 你要测试的类的头文件

// 测试宏变量替换函数
TEST(MacroVariableUtilTest, Replace) {
  std::unordered_map<std::string, std::string> variableMaps = {
      {"ACTION_ID", "12345"},
      {"REQUEST_ID", "abcde"}
  };
  std::string templateLink = "http://example.com/api?aid=__ACTION_ID__&rid=__REQUEST_ID__";
  std::string expected = "http://example.com/api?aid=12345&rid=abcde";
  ASSERT_EQ(expected, MacroVariableUtil::replace(templateLink, variableMaps)
  );
}

// 测试实际变量检查函数
TEST(MacroVariableUtilTest, IsRealVariable) {
  ASSERT_TRUE(MacroVariableUtil::isRealVariable("SOME_VARIABLE"));
  ASSERT_FALSE(MacroVariableUtil::isRealVariable("__MACRO_VARIABLE__"));
}

