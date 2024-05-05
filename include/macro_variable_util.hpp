//
// Created by xuantong on 2024/3/23.
//

#ifndef XUANOCPX_INCLUDE_MACROVARIABLEUTIL_HPP_
#define XUANOCPX_INCLUDE_MACROVARIABLEUTIL_HPP_
#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <random>

bool isMacroVariable(const std::string &value) {
  // 判断是否是宏变量，具体实现依赖于宏变量的定义
  return value.find("__") != std::string::npos;
}

class MacroVariableUtil {
 public:
  // 使用宏变量替换模板链接
  // SFINAE（Substitution Failure Is Not An Error）
  // 避免传入的参数不是std::unordered_map/std::map类型
  template<typename MapType>
  static auto replace(const std::string &templateLink, const MapType &variableMaps)
  -> decltype(variableMaps.begin()->first, variableMaps.begin()->second, std::string()) {
    std::string result = templateLink;
    for (const auto &pair : variableMaps) {
      std::string macro = "__" + pair.first + "__";
      size_t start_pos;
      while ((start_pos = result.find(macro)) != std::string::npos) {
        result.replace(start_pos, macro.length(), pair.second);
      }
    }
    return result;
  }

  // 将链接中不存在的宏变量替换为空
  static std::string replaceMacroVariable(const std::string &str, const std::string &replaceStr) {
    std::string result = str;
    size_t start_pos = 0;
    while ((start_pos = result.find("__", start_pos)) != std::string::npos) {
      size_t end_pos = result.find("__", start_pos + 2);
      if (end_pos != std::string::npos) {
        result.replace(start_pos, end_pos - start_pos + 2, replaceStr);
        start_pos = end_pos + 2;
      } else {
        break;
      }
    }
    return result;
  }

  // 检查是否是真正的变量（不是宏变量）
  static bool isRealVariable(const std::string &variable) {
    return !isMacroVariable(variable) && !variable.empty();
  }

  // 其他必要的方法和属性
};
#endif //XUANOCPX_INCLUDE_MACROVARIABLEUTIL_HPP_
