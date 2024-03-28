//
// Created by Dell on 2024/3/28 0028.
//
#include <gtest/gtest.h>
#include "enum_util.hpp"
#include "string_utils.h"
#include "MurmurHash3.h"

TEST(MurmurHash3_x86_128, Hash) {
    const char *key = string_utils::randomString(10).c_str();
    uint32_t seed = 56375; // Seed can be any integer value.
    uint32_t hash[4];
    MurmurHash3_x86_128(key, strlen(key), seed, &hash);
    std::stringstream ss;
    for (int i = 0; i < 4; i++) {
        ss << std::hex << std::setw(8) << std::setfill('0') << hash[i];
    }
    std::cout << "Hash0: " << hash << std::endl;
    std::cout << "Hash: " << ss.str() << std::endl;
}