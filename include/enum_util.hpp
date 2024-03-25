//
// Created by Dell on 2024/3/25 0025.
//

#ifndef XUANOCPX_ENUM_UTIL_HPP
#define XUANOCPX_ENUM_UTIL_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <type_traits>


// 通用枚举转换工具类
class EnumUtil {
public:
    template<typename T, size_t N>
    /**
     *
     * @tparam T 枚举类型
     * @tparam N 枚举长度
     * @param strValue 传入的字符枚举数据
     * @param defaultValue 默认返回的数据
     * @param validValues 需要转换的有效枚举集合类
     * @return  枚举数据
     *
     *  validValues数据示例：
     *  const std::array<std::underlying_type_t<ActionType>, 3> validActionTypeValues = {
     *   static_cast<std::underlying_type_t<ActionType>>(ActionType::Click),
     *   static_cast<std::underlying_type_t<ActionType>>(ActionType::Exposure),
     *   static_cast<std::underlying_type_t<ActionType>>(ActionType::CallBack)
     *  }
     */
    static T StringToEnum(const std::string &strValue, T defaultValue,
                          const std::array<std::underlying_type_t<T>, N> &validValues) {
        static_assert(std::is_enum<T>::value, "T must be an enum type");

        std::underlying_type_t<T> intValue;
        try {
            intValue = std::stoi(strValue);
        } catch (...) {
            return defaultValue;
        }

        if (std::find(validValues.begin(), validValues.end(), intValue) != validValues.end()) {
            return static_cast<T>(intValue);
        }

        return defaultValue;
    }
};


#endif //XUANOCPX_ENUM_UTIL_HPP
