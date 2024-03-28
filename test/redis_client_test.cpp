//
// Created by Dell on 2024/3/28 0028.
//
#include "redis_client.hpp"
#include <gtest/gtest.h>

class RedisClientTest : public ::testing::Test {
protected:
    RedisClient *client;

    // 在每个测试用例前执行
    void SetUp() override {
        // 连接到本地的Redis服务，确保Redis服务在默认端口16399上运行
        client = new RedisClient("127.0.0.1", 16399);
        // 清理Redis数据库
        client->set("FLUSHALL", "");
    }

    // 在每个测试用例后执行
    void TearDown() override {
        delete client;
    }
};

// 测试set和get方法
TEST_F(RedisClientTest, SetAndGet) {
    std::string key = "testKey";
    std::string value = "testValue";

    // 测试set方法
    ASSERT_TRUE(client->set(key, value));

    // 测试get方法
    EXPECT_EQ(client->get(key), value);

    // 测试默认值
    EXPECT_EQ(client->get("nonexistentKey", "defaultValue"), "defaultValue");
}

// 测试set方法的过期时间功能
TEST_F(RedisClientTest, SetWithExpiration) {
    std::string key = "testKeyWithExpiration";
    std::string value = "testValue";

    // 设置键值对和过期时间
    ASSERT_TRUE(client->set(key, value, RedisClient::ONE_SECOND));

    // 立即获取键值对，应该存在
    EXPECT_EQ(client->get(key), value);

    // 等待键值对过期
    sleep(RedisClient::ONE_SECOND + 1); // sleep for ttl+1 seconds to ensure the key has expired

    // 再次获取键值对，应该不存在
    EXPECT_EQ(client->get(key), "");
}


