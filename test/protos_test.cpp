//
// Created by Dell on 2024/3/28 0028.
//
#include "tracer_req.pb.h"
#include "gtest/gtest.h"
#include "string_utils.h"

class ProtoTest : public ::testing::Test {
 protected:
  // Test initialization code goes here.
};

TEST_F(ProtoTest, DefaultConstructor) {
  tracer::TracerReq msg;
  EXPECT_TRUE(msg.android_id_md5().empty());
}

TEST_F(ProtoTest, SetterGetter) {
  tracer::TracerReq msg;
  std::string action_id = string_utils::randomString(5);
  msg.set_action_id(action_id);
  EXPECT_EQ(action_id, msg.action_id());
  EXPECT_TRUE(msg.android_id_md5().empty()); // 如果android_id_md5是字符串类型，未设置时应该为空字符串
  EXPECT_EQ(tracer::ActionType::UNKNOWN_ACTION, msg.action()); // 枚举设置为默认的0
}

TEST_F(ProtoTest, Serialization) {
  tracer::TracerReq msg;
  msg.set_android_id("Hello, world!");

  // Serialize to string.
  std::string serialized;
  ASSERT_TRUE(msg.SerializeToString(&serialized));

  // Deserialize from string.
  tracer::TracerReq deserialized_msg;
  ASSERT_TRUE(deserialized_msg.ParseFromString(serialized));

  // Check content.
  EXPECT_EQ(msg.android_id(), deserialized_msg.android_id());
}
