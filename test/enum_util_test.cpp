//
// Created by Dell on 2024/3/25 0025.
//
#include <gtest/gtest.h>
#include "enum_util.hpp"
#include "tracer_http_handler.h"
#include "string_utils.h"

TEST(EnumUtil, StringToEnum) {
    EXPECT_EQ(EnumUtil::StringToEnum("1", tracer::req::ActionType::UNKNOWN_ACTION, VALID_ACTIONTYPE_VALUES),
              tracer::req::ActionType::CLICK_ACTION);
    EXPECT_EQ(EnumUtil::StringToEnum("2", tracer::req::ActionType::UNKNOWN_ACTION, VALID_ACTIONTYPE_VALUES),
              tracer::req::ActionType::EXPOSURE_ACTION);
    EXPECT_EQ(EnumUtil::StringToEnum("3", tracer::req::ActionType::UNKNOWN_ACTION, VALID_ACTIONTYPE_VALUES),
              tracer::req::ActionType::CALLBACK_ACTION);
    EXPECT_EQ(EnumUtil::StringToEnum("4", tracer::req::ActionType::UNKNOWN_ACTION, VALID_ACTIONTYPE_VALUES),
              tracer::req::ActionType::UNKNOWN_ACTION);
    EXPECT_EQ(EnumUtil::StringToEnum(string_utils::randomString(5), tracer::req::ActionType::UNKNOWN_ACTION, VALID_ACTIONTYPE_VALUES),
              tracer::req::ActionType::UNKNOWN_ACTION);
}