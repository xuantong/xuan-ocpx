//
// Created by shangjun on 2024/3/2 /0002.
//

#include "string_utils.h"

namespace string_utils {
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
        const size_t max_index =
                sizeof(string_utils::DEFAULT_RANDOM_CHARSET) - 2; // 减去1是因为数组是从0开始索引，再减去1是因为最后一个是空字符'\0'
        std::string random_string;
        random_string.reserve(length);

        // 初始化随机数生成器
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, max_index);

        // 生成随机字符串
        for (size_t i = 0; i < length; ++i) {
            random_string += string_utils::DEFAULT_RANDOM_CHARSET[dist(rng)];
        }

        return random_string;
    }

    std::string urlEncode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (auto i = value.begin(), n = value.end(); i != n; ++i) {
            std::string::value_type c = (*i);

            // 保留字母数字字符和其他非标准字符
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }

            // 百分比编码非保留字母数字字符
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    std::string urlDecode(const std::string &value) {
        std::ostringstream decoded;
        size_t i = 0;
        while (i < value.length()) {
            if (value[i] == '%' && i + 2 < value.length()) {
                // 提取两个字符的十六进制字符串
                std::string hex_str = value.substr(i + 1, 2);
                char decoded_char = static_cast<char>(strtol(hex_str.c_str(), nullptr, 16));
                decoded << decoded_char;
                i += 3; // 跳过当前的百分号和两个十六进制字符
            } else if (value[i] == '+') { // 处理空格
                decoded << ' ';
                i++;
            } else {
                decoded << value[i++];
            }
        }
        return decoded.str();
    }


    std::string hash(const std::string &value) {
        const char *key = value.c_str();
        uint32_t hash[4];
        MurmurHash3_x86_128(key, strlen(key), string_utils::HASH_SEED, &hash);
        std::stringstream ss;
        for (int i = 0; i < 4; i++) {
            ss << std::hex << std::setw(8) << std::setfill('0') << hash[i];
        }
        return ss.str();
    }

    std::string truncate_string(const std::string &str, size_t max_length) {
        if (str.length() > max_length && max_length >= 3) {
            return str.substr(0, max_length - 3) + "...";
        }
        return str;
    }
}