//
// Created by Dell on 2024/3/25 0025.
//
#include "tracer_http_handler.h"

logpp::Logger TracerHttpHandler::logger_ = logpp::Logger("TracerHttpHandler");

int TracerHttpHandler::tracer(HttpRequest *req, HttpResponse *resp) {
    req->query_params;
    req->url;
    return 0;
}