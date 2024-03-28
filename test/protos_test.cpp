////
//// Created by Dell on 2024/3/28 0028.
////
//#include "tracer_req.pb.h"
//#include "gtest/gtest.h"
//
//class ProtoTest : public ::testing::Test {
//protected:
//    // Test initialization code goes here.
//};
//
//TEST_F(ProtoTest, DefaultConstructor) {
//    example::SimpleMessage msg;
//    EXPECT_TRUE(msg.content().empty());
//}
//
//TEST_F(ProtoTest, SetterGetter) {
//    example::SimpleMessage msg;
//    msg.set_content("Hello, world!");
//    EXPECT_EQ("Hello, world!", msg.content());
//}
//
//TEST_F(ProtoTest, Serialization) {
//    example::SimpleMessage msg;
//    msg.set_content("Hello, world!");
//
//    // Serialize to string.
//    std::string serialized;
//    ASSERT_TRUE(msg.SerializeToString(&serialized));
//
//    // Deserialize from string.
//    example::SimpleMessage deserialized_msg;
//    ASSERT_TRUE(deserialized_msg.ParseFromString(serialized));
//
//    // Check content.
//    EXPECT_EQ(msg.content(), deserialized_msg.content());
//}
