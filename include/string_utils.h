//
// Created by shangjun on 2024/3/2 /0002.
//

#ifndef SPM_PLATE_STRING_UTILS_HPP
#define SPM_PLATE_STRING_UTILS_HPP
#pragma once

#include <iostream>
#include <sstream>
#include "vector"
#include "functional"
#include "openssl/md5.h"
#include "string.h"
#include <random>
#include <string>
#include <ctime>
#include <iomanip>
#include "MurmurHash3.h"
#include "assert.h"

namespace string_utils {
    static const char DEFAULT_DELIMITER = ',';
    static const std::string DEFAULT_SLAT_STRING = "8c12a6d9-e758-46b5-84d1-8c5482bdea44";
    static const char DEFAULT_RANDOM_CHARSET[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const uint32_t HASH_SEED = 76329;

    template<typename T>
    std::vector<T>
    splitStringToVector(const std::string &str, char delimiter, std::function<T(const std::string &)> converter) {
        std::vector<T> result;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            try {
                // 使用回调函数将字符串转换成所需类型并添加到结果向量中
                result.push_back(converter(token));
            } catch (const std::exception &e) {
                // 处理转换过程中可能出现的任何异常
                std::cerr << "Conversion error: " << e.what() << std::endl;
            }
        }

        return result;
    }

    /**
     * 生成带盐的MD5字符串，用于验签
     * @param data
     * @param salt
     * @return
     */
    std::string generateMD5HashWithInterleavedSalt(const std::string &data, const std::string &salt);

    /**
     * 生成随机的字符串
     * @param length
     * @return
     */
    std::string randomString(size_t length);

    /**
     * url encode
     * @param value
     * @return
     */
    std::string urlEncode(const std::string &value);


    /**
     * url decode
     * @param value
     * @return
     */
    std::string urlDecode(const std::string &value);

    /**
     * 生成对应字符的hash数据
     * @param value
     * @return
     */
    std::string hash(const std::string &value);

    /**
     * 固定截断字符，用于输出使用
     * @param str 源字符串
     * @param max_length 截断长度
     * @return
     */
    std::string truncate_string(const std::string &str, size_t max_length = 256);
}
#endif //SPM_PLATE_STRING_UTILS_HPP
