//
// Created by shangjun on 2024/3/2 /0002.
//

#ifndef SPM_PLATE_STRING_UTILS_HPP
#define SPM_PLATE_STRING_UTILS_HPP

#include <iostream>
#include <sstream>
#include "vector"
#include "functional"
#include "openssl/md5.h"
#include <random>
#include <string>
#include <ctime>

static const char DEFAULT_DELIMITER = ',';
static const std::string DEFAULT_SLAT_STRING = "8c12a6d9-e758-46b5-84d1-8c5482bdea44";
static const char DEFAULT_RANDOM_CHARSET[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

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

// 将盐穿插到数据中的函数
std::string interleaveSaltWithData(const std::string &data, const std::string &salt) {
    std::string result;
    size_t dataLen = data.length();
    size_t saltLen = salt.length();
    size_t maxLen = std::max(dataLen, saltLen);

    for (size_t i = 0; i < maxLen; ++i) {
        if (i < dataLen) {
            result += data[i];  // 取一个字符来自数据
        }
        if (i < saltLen) {
            result += salt[i];  // 取一个字符来自盐
        }
    }

    return result;
}

// 生成带盐的MD5哈希值的函数
std::string generateMD5HashWithInterleavedSalt(const std::string &data, const std::string &salt) {
    std::string dataWithSalt = interleaveSaltWithData(data, salt);

    // 生成带盐的MD5哈希值
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char *>(dataWithSalt.c_str()), dataWithSalt.size(), digest);
    char mdString[33];
    for (int i = 0; i < 16; ++i) {
        sprintf(&mdString[i * 2], "%02x", (unsigned int) digest[i]);
    }
    return std::string(mdString);
}

std::string randomString(size_t length) {
    const size_t max_index = sizeof(DEFAULT_RANDOM_CHARSET) - 2; // 减去1是因为数组是从0开始索引，再减去1是因为最后一个是空字符'\0'
    std::string random_string;
    random_string.reserve(length);

    // 初始化随机数生成器
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, max_index);

    // 生成随机字符串
    for (size_t i = 0; i < length; ++i) {
        random_string += DEFAULT_RANDOM_CHARSET[dist(rng)];
    }

    return random_string;
}

#endif //SPM_PLATE_STRING_UTILS_HPP
