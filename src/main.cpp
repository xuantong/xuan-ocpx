//
// Created by xuantong on 2024/3/10.
//
#include <logpp/logpp.hpp>
#include "main_http_server.hpp"

void config_logger() {
  logpp::LogUtil &util = logpp::LogUtil::getInstance();
//    util.setRootLogLevel(logpp::LogLevel::WARN);
//  util.setRootLogLevel(logpp::LogLevel::INFO);
  util.setRootLogLevel(logpp::LogLevel::TRACE);
//  util.setRootLogLevel(LogLevel::DEBUG);
}

int main(int argc, char **argv) {
  config_logger();
  logpp::Logger logger("main");

  http_server(argc, argv);

  return 0;
}