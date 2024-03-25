//
// Created by Dell on 2024/3/25 0025.
//
#include "tracer_http_handler.h"

logpp::Logger TracerHttpHandler::logger_ = logpp::Logger("TracerHttpHandler");

int TracerHttpHandler::tracer(HttpRequest *req, HttpResponse *resp) {
    logger_.info("TracerHttpHandler tracer");
    // 转换出对应对象数据
    TracerReq tracerReq = extractTracerReq(*req);
    return 0;
}