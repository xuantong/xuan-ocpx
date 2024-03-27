//
// Created by xuantong on 2024/3/27.
//

#ifndef XUANOCPX_INCLUDE_REDIS_CLIENT_HPP_
#define XUANOCPX_INCLUDE_REDIS_CLIENT_HPP_
#include <hiredis/hiredis.h>
#include <string>
#include <iostream>

class RedisUtil {
 public:
  // 定义一些失效时长的常量（单位：秒）
  static constexpr int FIVE_MINUTES = 5 * 60;
  static constexpr int FIFTEEN_MINUTES = 15 * 60;
  static constexpr int ONE_HOUR = 60 * 60;
  static constexpr int EIGHT_HOURS = 8 * 60 * 60;
  static constexpr int ONE_DAY = 24 * 60 * 60;

  RedisUtil(const std::string &host, int port) : host_(host), port_(port) {
    _connect();
  }

  ~RedisUtil() {
    if (context_ != nullptr) {
      redisFree(context_);
    }
  }

  bool set(const std::string &key, const std::string &value) {
    return set(key, value, 0); // 默认不设置过期时间
  }

  bool set(const std::string &key, const std::string &value, int ttl) {
    if (context_ == nullptr) return false;

    redisReply *reply;
    if (ttl > 0) {
      // 设置带有失效时长的键值对
      reply = (redisReply *) redisCommand(context_, "SET %s %s EX %d", key.c_str(), value.c_str(), ttl);
    } else {
      // 设置不带失效时长的键值对
      reply = (redisReply *) redisCommand(context_, "SET %s %s", key.c_str(), value.c_str());
    }
    bool isSuccess = (reply != nullptr) && (reply->type == REDIS_REPLY_STATUS) && (std::string(reply->str) == "OK");
    freeReplyObject(reply);
    return isSuccess;
  }

  std::string get(const std::string &key, const std::string &default_value = "") {
    if (context_ == nullptr) return default_value;

    redisReply *reply = (redisReply *) redisCommand(context_, "GET %s", key.c_str());
    std::string value = default_value;
    if (reply != nullptr && reply->type == REDIS_REPLY_STRING) {
      value = reply->str;
    }
    freeReplyObject(reply);
    return value;
  }

 private:
  void _connect() {
    struct timeval timeout = {1, 500000}; // 1.5 seconds
    context_ = redisConnectWithTimeout(host_.c_str(), port_, timeout);
    if (context_ == nullptr || context_->err) {
      if (context_) {
        std::cerr << "Connection error: " << context_->errstr << std::endl;
        redisFree(context_);
        context_ = nullptr;
      } else {
        std::cerr << "Connection error: can't allocate redis context" << std::endl;
      }
    }
  }

  redisContext *context_ = nullptr;
  std::string host_;
  int port_;
};
#endif //XUANOCPX_INCLUDE_REDIS_CLIENT_HPP_
