//
// Created by xuantong on 2024/3/24.
//
#include <gtest/gtest.h>
#include "string_utils.h"

using namespace string_utils;
// 测试URL编码函数
TEST(string_utils_test, urlEncode) {
    std::string original = "This is a test: 1+2 = 3!";
    std::string expected = "This%20is%20a%20test%3A%201%2B2%20%3D%203%21";
    ASSERT_EQ(expected, urlEncode(original));
}

TEST(string_utils_test, urlDecode) {
    std::string data = "%E4%B8%AD%E5%9B%BD%E6%9C%89%E5%A4%9A%E5%B0%91%E4%B8%AA%E7%9C%81%E5%95%8A2";
    std::string expected = "中国有多少个省啊2";
    ASSERT_EQ(expected, urlDecode(data));
    std::string expected2 = "wo是中国人2啊是不是bbc一直好用。";
    ASSERT_EQ(expected2, urlDecode(urlEncode(expected2)));
}

TEST(string_utils_test, randomString) {
    for (int i = 10; i < 20; ++i) {
        std::string expected = randomString(i);
        ASSERT_EQ(expected.length(), i);
    }
}

TEST(string_utils_test, generateMD5HashWithInterleavedSalt) {
    std::string testStr = randomString(10);
    std::string tmp_slat = randomString(20);
    std::string hash1 = generateMD5HashWithInterleavedSalt(testStr, tmp_slat);
    std::string hash2 = generateMD5HashWithInterleavedSalt(testStr, tmp_slat);

    // 测试相同输入的哈希值是否相同
    ASSERT_EQ(hash1, hash2);

    std::string differentStr = randomString(10);
    std::string hash3 = generateMD5HashWithInterleavedSalt(differentStr, tmp_slat);

    // 测试不同输入的哈希值是否不同
    ASSERT_NE(hash1, hash3);

    for (int i = 0; i < 20; ++i) {
        std::string newStr = randomString(10);
        std::string newHash = generateMD5HashWithInterleavedSalt(newStr, tmp_slat);
        ASSERT_NE(hash1, newHash);
    }
}

