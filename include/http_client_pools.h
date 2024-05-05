// Created by Dell on 2024/4/26 0026.
//

#ifndef XUANOCPX_HTTP_CLIENT_POOLS_H
#define XUANOCPX_HTTP_CLIENT_POOLS_H

#include "hv/requests.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cassert>

using namespace hv;

//一个基于libhv的http客户端连接池
class HttpClientPools {
private:
    std::queue<std::shared_ptr<HttpRequest>> pool;
    std::mutex mtx;
    std::condition_variable cv;

    std::shared_ptr<HttpRequest> get() {
        std::unique_lock<std::mutex> lock(mtx);
        while (pool.empty()) {
            cv.wait(lock);
        }
        auto client = pool.front();
        pool.pop();
        return client;
    }

    void put(std::shared_ptr<HttpRequest> client) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            pool.push(client);
        }
        cv.notify_one();
    }

    bool isValidUrl(const char* url) {
        return url && *url;
    }

public:
    HttpClientPools(int size = 10) {
        assert(size > 0);
        for (int i = 0; i < size; i++) {
            pool.push(std::make_shared<HttpRequest>());
        }
    }

    std::string get(const char *url) {
        if (!isValidUrl(url)) {
            return "";
        }

        auto req = get();
        req->method = HTTP_GET;
        req->url = url;
        auto resp = requests::request(req);
        put(req);
        if (resp == nullptr) {
            return "";
        }
        return resp->body;
    }

    std::string post(const char *url, const char *body) {
        if (!isValidUrl(url)) {
            return "";
        }

        auto req = get();
        req->method = HTTP_POST;
        req->url = url;
        req->body = body;
        auto resp = requests::request(req);
        put(req);
        if (resp == nullptr) {
            return "";
        }
        return resp->body;
    }

    void send(const char *url, const char *body = nullptr, http_method method = HTTP_GET) {
        if (!isValidUrl(url)) {
            return;
        }

        auto req = get();
        req->method = method;
        req->url = url;
        if (body && method == HTTP_POST) {
            req->body = body;
        }
        requests::request(req); // 不需要处理响应
        put(req);
    }
};

#endif //XUANOCPX_HTTP_CLIENT_POOLS_H
