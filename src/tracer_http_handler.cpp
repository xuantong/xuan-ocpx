//
// Created by Dell on 2024/3/25 0025.
//
#include "tracer_http_handler.h"

logpp::Logger TracerHttpHandler::logger_ = logpp::Logger("TracerHttpHandler");

int TracerHttpHandler::tracer(HttpRequest *req, HttpResponse *resp) {
    logger_.info("TracerHttpHandler tracer");
    // 转换出对应对象数据
    TracerReq tracerReq = extractTracerReq(*req);
    // 序列化 TracerReq 对象为 JSON 字符串
    std::string serialized = nlohmann::json(tracerReq).dump();
    std::string hash_key = string_utils::hash(serialized);
    // 将这个对象转换并存储
    level_db_wrapper_.Put(hash_key, serialized);
    // 将数据缓存到redis上，保持一天，一般来说单次请求，当日内一般会转化返回，如果一天未转化则放弃
    redisClient_.set(hash_key, serialized, RedisClient::ONE_DAY);

    // 根据配置文件提取回传数据对象

    return 200;
}

int TracerHttpHandler::callback(HttpRequest *req, HttpResponse *resp) {
    logger_.info("TracerHttpHandler callback");
    return 200;
}