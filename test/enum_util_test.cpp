//
// Created by Dell on 2024/3/25 0025.
//
#include <gtest/gtest.h>
#include "enum_util.hpp"
#include "tracer_http_handler.h"
#include "string_utils.h"

TEST(EnumUtil, StringToEnum) {
    EXPECT_EQ(EnumUtil::StringToEnum("1", ActionType::Unknown, VALID_ACTIONTYPE_VALUES),
              ActionType::Click);
    EXPECT_EQ(EnumUtil::StringToEnum("2", ActionType::Unknown, VALID_ACTIONTYPE_VALUES),
              ActionType::Exposure);
    EXPECT_EQ(EnumUtil::StringToEnum("3", ActionType::Unknown, VALID_ACTIONTYPE_VALUES),
              ActionType::CallBack);
    EXPECT_EQ(EnumUtil::StringToEnum("4", ActionType::Unknown, VALID_ACTIONTYPE_VALUES),
              ActionType::Unknown);
    EXPECT_EQ(EnumUtil::StringToEnum(string_utils::randomString(5), ActionType::Unknown, VALID_ACTIONTYPE_VALUES),
              ActionType::Unknown);
}