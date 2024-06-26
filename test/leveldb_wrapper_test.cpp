//
// Created by xuantong on 2024/3/24.
//
#include "leveldb_wrapper.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include "string_utils.h"
#include "tracer_req.pb.h"

namespace {

// 测试夹具，用于设置和清理LevelDB环境。
class LevelDBWrapperTest : public ::testing::Test {
 protected:
  std::string db_path;
  LevelDBWrapper *db_wrapper;

  // 在每个测试用例之前调用。
  void SetUp() override {
    // 创建一个临时目录用于测试数据库。
    db_path = std::filesystem::temp_directory_path() / "leveldb_test_db";
    std::filesystem::create_directories(db_path);
    // 初始化LevelDBWrapper对象。
    db_wrapper = new LevelDBWrapper(db_path);
  }

  // 在每个测试用例之后调用。
  void TearDown() override {
    // 删除LevelDBWrapper对象。
    delete db_wrapper;
    // 清理测试数据库目录。
    std::filesystem::remove_all(db_path);
  }
};

// 测试Put和Get方法。
TEST_F(LevelDBWrapperTest, PutAndGet) {
  std::string key = string_utils::randomString(10);
  std::string value = string_utils::randomString(12);

  // 测试Put方法。
  EXPECT_NO_THROW(db_wrapper->Put(key, value));

  // 测试Get方法。
  std::string returned_value;
  EXPECT_NO_THROW(returned_value = db_wrapper->Get(key));
  EXPECT_EQ(value, returned_value);
}

// 测试Delete方法。
TEST_F(LevelDBWrapperTest, Delete) {
  std::string key = string_utils::randomString(10);
  std::string value = string_utils::randomString(12);

  // 先插入一个键值对。
  db_wrapper->Put(key, value);

  // 然后删除。
  EXPECT_NO_THROW(db_wrapper->Delete(key));

  // 尝试获取已删除的键值对，应该抛出异常。
  EXPECT_THROW(db_wrapper->Get(key), std::runtime_error);
}

// 测试WriteBatch方法。
TEST_F(LevelDBWrapperTest, WriteBatch) {
  std::vector<std::pair<std::string, std::string>> updates = {
      {"key3", "value3"},
      {"key4", "value4"}
  };

  // 批量写入键值对。
  EXPECT_NO_THROW(db_wrapper->WriteBatch(updates));

  // 验证是否写入成功。
  for (const auto &pair : updates) {
    std::string value;
    EXPECT_NO_THROW(value = db_wrapper->Get(pair.first));
    EXPECT_EQ(pair.second, value);
  }
}
// 测试Protobuf Put和Get。
TEST_F(LevelDBWrapperTest, PutAndGetProtobuf) {
  tracer::req::TracerReq original_msg;
  original_msg.set_android_id("Test Android ID");
  original_msg.set_action_id(string_utils::randomString(5));
  original_msg.set_action(tracer::req::ActionType::CLICK_ACTION);

  // 序列化protobuf消息。
  std::string serialized;
  ASSERT_TRUE(original_msg.SerializeToString(&serialized));

  // 定义一个键用于LevelDB存储。
  std::string key = string_utils::hash(serialized);

  // 存储序列化的protobuf字符串到LevelDB。
  db_wrapper->Put(key, serialized);

  // 从LevelDB读取序列化的protobuf字符串。
  std::string retrieved_serialized = db_wrapper->Get(key);

  // 反序列化字符串到protobuf消息。
  tracer::req::TracerReq retrieved_msg;
  ASSERT_TRUE(retrieved_msg.ParseFromString(retrieved_serialized));

  // 验证原始消息和检索的消息是否一致。
  EXPECT_EQ(original_msg.android_id(), retrieved_msg.android_id());
  EXPECT_EQ(original_msg.action_id(), retrieved_msg.action_id());
  EXPECT_EQ(original_msg.action(), retrieved_msg.action());
}

}  // namespace
