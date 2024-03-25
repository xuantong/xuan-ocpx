//
// Created by xuantong on 2024/1/5.
//

#include <cstdio>
#include <thread>
#include <queue>
#include <vector>
#include <filesystem>
#include <iostream>
#include <logpp/logpp.hpp>
#include <dirent.h>
#include <fstream>

#include "cmdline.h"
#include "hv/HttpServer.h"
#include <hv/HttpMessage.h>
#include "hv/httpdef.h"

#include "global_http_handler.hpp"
#include "string_utils.h"
#include "tracer_http_handler.h"

using json = nlohmann::json;

static int response_status(HttpResponse *resp, int code = 200, const char *message = nullptr) {
    if (message == nullptr) message = http_status_str((enum http_status) code);
    resp->Set("code", code);
    resp->Set("message", message);
    return code;
}

// 检查哈希值是否过期的函数，expirationTime以秒为单位
static bool isHashExpired(const std::string &hash, long timestamp, long expirationTime) {
    // 将传入的时间戳转换为时间点
    std::chrono::system_clock::time_point timeStampPoint =
            std::chrono::system_clock::from_time_t(timestamp);

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - timeStampPoint).count();
    return duration > expirationTime;
}

#include <csignal>

// 全局变量，用于控制程序退出
volatile std::sig_atomic_t g_terminate = 0;

// 信号处理函数
void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "Received signal " << signal << ", will terminate." << std::endl;
        g_terminate = 1;
    }
}

/**
 * 启动一个http服务
 * @param argc
 * @param argv
 */
void http_server(int argc, char **argv) {
    logpp::Logger logger("http_server");
    logger.info("start http server ...");

    // 设置信号处理程序
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    cmdline::parser args_parser;
    args_parser.add<int>("http_port", 'p', "启动http对应的端口号", false, 8080,
                         cmdline::range(1000, 35565));


    // 入参检查
    args_parser.parse_check(argc, argv);
    // 提取入参
    int http_port = args_parser.get<int>("http_port");
    // http启动服务
    hv::HttpService router;
    router.preprocessor = GlobalHttpHandler::preprocessor;
//  router.AllowCORS();
//  router.Use(GlobalHttpHandler::Authorization);

    router.GET("/", [](HttpRequest *req, HttpResponse *resp) {
        return response_status(resp, HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED);
    });

    router.GET("/ping", [](HttpRequest *req, HttpResponse *resp) {
        return resp->String("pong");
    });

    // 创建一个 const std::function 对象来存储 lambda 表达式
    const std::function<int(const std::string &)> string2ints_converter = [](const std::string &s) -> int {
        return std::stoi(s);
    };

    // 字符串转换器，实际上对于字符串到字符串的转换，可以直接返回输入的字符串本身
    const std::function<string(const std::string &)> string2string_converter = [](
            const std::string &str) -> std::string {
        return str;
    };

    // tracer请求链接
    router.GET("/tracer", TracerHttpHandler::tracer);


    // 创建并启动HTTP服务器
    hv::HttpServer server;
    server.service = &router;
    server.setHost("0.0.0.0");
    server.setPort(http_port);
    server.start();
    logger.info("http is start success...");

    // 清空之前缓冲区数据
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//    int c;
//    while ((c = getchar()) != '\n' && c != EOF);
//    logger.info("http server is closing.");

    logger.info("http server is started and running.");

    // 无限循环，直到收到退出信号
    while (!g_terminate) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    logger.info("http server is closing.");
}

