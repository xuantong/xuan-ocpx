//
// Created by xuantong on 2024/3/25.
//

#ifndef XUANOCPX_INCLUDE_CACHE_MANAGER_HPP_
#define XUANOCPX_INCLUDE_CACHE_MANAGER_HPP_
#include <unordered_map>
#include <shared_mutex>
#include <chrono>
#include <future>
#include <functional>
#include <thread>

template<typename Key, typename Value>
class CacheManager {
 public:
  using LoadFunction = std::function<Value(const Key &)>;
  using Clock = std::chrono::steady_clock;

  CacheManager(LoadFunction loader, std::chrono::milliseconds refresh_interval)
      : loader_(std::move(loader)), refresh_interval_(refresh_interval) {
    // 启动后台刷新线程
    refresh_thread_ = std::async(std::launch::async, [this] { this->RefreshLoop(); });
  }

  ~CacheManager() {
    // 通知刷新线程退出
    running_ = false;
    refresh_thread_.wait();
  }

  Value Get(const Key &key) {
    std::shared_lock lock(mutex_);
    auto it = cache_.find(key);
    if (it != cache_.end() && Clock::now() < it->second.second) {
      return it->second.first;
    } else {
      // 加载数据并更新缓存
      lock.unlock();
      Value value = loader_(key);
      UpdateCache(key, value);
      return value;
    }
  }

  void UpdateCache(const Key &key, const Value &value) {
    std::unique_lock lock(mutex_);
    cache_[key] = std::make_pair(value, Clock::now() + refresh_interval_);
  }

 private:
  void RefreshLoop() {
    while (running_) {
      std::this_thread::sleep_for(refresh_interval_);
      RefreshCache();
    }
  }

  void RefreshCache() {
    std::unique_lock lock(mutex_);
    for (auto &[key, value] : cache_) {
      if (Clock::now() >= value.second) {
        // 重新加载数据并更新过期时间
        value.first = loader_(key);
        value.second = Clock::now() + refresh_interval_;
      }
    }
  }

  std::unordered_map<Key, std::pair<Value, Clock::time_point>> cache_;
  LoadFunction loader_;
  std::chrono::milliseconds refresh_interval_;
  std::shared_mutex mutex_; // 允许并发读取但互斥写入的互斥锁
  std::future<void> refresh_thread_; // 后台刷新线程
  std::atomic<bool> running_{true}; // 控制刷新线程的运行
};

#endif //XUANOCPX_INCLUDE_CACHE_MANAGER_HPP_
