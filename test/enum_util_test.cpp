//
// Created by Dell on 2024/3/25 0025.
//
#include <gtest/gtest.h>
#include "enum_util.hpp"
#include "tracer_http_handler.h"
#include "string_utils.h"

TEST(EnumUtil, StringToEnum) {
    EXPECT_EQ(EnumUtil::StringToEnum("1", tracer::ActionType::UNKNOWN, VALID_ACTIONTYPE_VALUES),
              tracer::ActionType::CLICK);
    EXPECT_EQ(EnumUtil::StringToEnum("2", tracer::ActionType::UNKNOWN, VALID_ACTIONTYPE_VALUES),
              tracer::ActionType::EXPOSURE);
    EXPECT_EQ(EnumUtil::StringToEnum("3", tracer::ActionType::UNKNOWN, VALID_ACTIONTYPE_VALUES),
              tracer::ActionType::CALLBACK);
    EXPECT_EQ(EnumUtil::StringToEnum("4", tracer::ActionType::UNKNOWN, VALID_ACTIONTYPE_VALUES),
              tracer::ActionType::UNKNOWN);
    EXPECT_EQ(EnumUtil::StringToEnum(string_utils::randomString(5), tracer::ActionType::UNKNOWN, VALID_ACTIONTYPE_VALUES),
              tracer::ActionType::UNKNOWN);
}