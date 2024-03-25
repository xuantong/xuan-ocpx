//
// Created by shangjun on 2024/2/24 /0024.
//

#ifndef SPM_PLATE_HTTPHANDLER_HPP
#define SPM_PLATE_HTTPHANDLER_HPP

#include "hv/HttpService.h"
#include "hv/json.hpp"
#include <string>
#include "hv/hbase.h"
#include "hv/htime.h"
#include "hv/hstring.h"
#include "hv/EventLoop.h" // import setTimeout, setInterval
#include "string_utils.h"

#define HTTP_HANDLER_AUTH_EXPIRED_TIME 30

using json = nlohmann::json;

class GlobalHttpHandler {
private:
    static logpp::Logger logger_;
    // 用于存储登录状态的md5hash数据值
    static std::map<std::string, std::chrono::system_clock::time_point> hashTimestamps_;
    // 需要校验登录的url
    static std::vector<std::string> need_auth_url_;

public :
    /**
     * 回发对应错误码
     * @param resp
     * @param code
     * @param message
     * @return
     */
    static int
    response_status(HttpResponse *resp, int code = 200, const std::string result = NULL, const char *message = NULL) {
        if (message == NULL) message = http_status_str((enum http_status) code);
        resp->Set("code", code);
        resp->Set("message", message);
        resp->Set("result", result);
        return code;
    }

    static int response_status(const HttpResponseWriterPtr &writer, int code = 200, const char *message = NULL) {
        response_status(writer->response.get(), code, message);
        writer->End();
        return code;
    }

    static int response_status(const HttpContextPtr &ctx, int code = 200, const char *message = NULL) {
        response_status(ctx->response.get(), code, message);
        ctx->send();
        return code;
    }

    static int errorHandler(const HttpContextPtr &ctx) {
        int error_code = ctx->response->status_code;
        return response_status(ctx, error_code);
    }

    static int preprocessor(HttpRequest *req, HttpResponse *resp) {
        // 记录数据请求
        logger_.info("client ip:{},port:{}", req->client_addr.ip.c_str(), req->client_addr.port);
        return HTTP_STATUS_NEXT;
    }

    static std::string genAuthKey(std::string str) {
        std::string genKey = generateMD5HashWithInterleavedSalt(
                str, DEFAULT_SLAT_STRING
        );
        //将生成时间插入
        hashTimestamps_.insert({genKey, std::chrono::system_clock::now()});
        return genKey;
    }

    // middleware
    // 认证部分
    static int Authorization(HttpRequest *req, HttpResponse *resp) {
        logger_.info("[Authorization] url is {}", req->path.c_str());

        std::string headers = "";
        req->DumpHeaders(headers);
        logger_.info("dumpHeaders is {}", headers);
        std::string token = req->GetHeader("Spmauthorization");
        logger_.warn("[Authorization] url is {} ,token is {} ...", req->path, token);

        if (std::find(need_auth_url_.begin(),
                      need_auth_url_.end(), req->path.c_str()) != need_auth_url_.end()) {
            logger_.warn("[Authorization] url is {} need Authorization...", req->path);
            std::string token = req->GetHeader("Spmauthorization");
            if (!token.empty()) {
                logger_.warn("[Authorization] Token is Not empty,token is {}", token);
                auto it = hashTimestamps_.find(token);
                auto now = std::chrono::system_clock::now();
                json return_res = {
                        {"success", false},
                        {"msg",     ""},
                        {"result",  ""}
                };

                // 检查token是否存在且时间未超过30分钟
                if (it != hashTimestamps_.end()) {
                    auto last_access_time = it->second;
                    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - last_access_time);
                    if (duration.count() <= HTTP_HANDLER_AUTH_EXPIRED_TIME) {
                        // token 存在且在有效期内
                        // 更新时间戳
                        it->second = now;
                        logger_.info("[Authorization] url:{} ,update success, Authorization success...", req->path);
                        // 允许继续执行后续操作
                        return HTTP_STATUS_NEXT;
                    } else {
                        // token 存在但已超时
                        logger_.warn("[Authorization] Token for url {} is expired.", req->path);
                    }
                    // token 验证失败，需要重新登录
                    return_res["result"] = "Token expired.";
                    resp->String(return_res.dump());
//                    response_status(resp, 10011, "Token expired or invalid, please login again.");
                    return HTTP_STATUS_ACCEPTED;
                } else {
                    // token 不存在
                    logger_.warn("[Authorization] Token for url {} does not exist.", req->path);
                }

                return_res["result"] = "Token invalid.";
                resp->String(return_res.dump());
                return HTTP_STATUS_ACCEPTED;
            } else {
                logger_.warn("url is {} Authorization is fail ,token is empty...", req->path);
                response_status(resp, 10011, "Miss Authorization header!");
                return HTTP_STATUS_UNAUTHORIZED;
            }
        }

        return HTTP_STATUS_NEXT;
    }
};

logpp::Logger GlobalHttpHandler::logger_ = logpp::Logger("GlobalHttpHandler");
std::map<std::string, std::chrono::system_clock::time_point> GlobalHttpHandler::hashTimestamps_;
std::vector<std::string> GlobalHttpHandler::need_auth_url_ = {
        "/modify_config",
        "/modify_passwd",
        "/get_config",
};

#endif //SPM_PLATE_HTTPHANDLER_HPP
