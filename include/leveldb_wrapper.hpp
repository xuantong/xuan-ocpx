//
// Created by xuantong on 2024/3/23.
//

#ifndef XUANOCPX_INCLUDE_LEVELDBWRAPPER_HPP_
#define XUANOCPX_INCLUDE_LEVELDBWRAPPER_HPP_
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <leveldb/cache.h>
#include <stdexcept>
#include <string>
#include <vector>

class LevelDBWrapper {
 public:
  LevelDBWrapper(const std::string &db_path) {
    leveldb::Options options;
    options.create_if_missing = true; // 默认创建
    //写操作首先会被写入内存中的缓冲区，达到一定大小后再统一写入磁盘。读多写少的场景不需要太大的写缓冲区。
    options.write_buffer_size = 4 * 1048576; // 设置较小的写缓冲区，4M
    options.max_open_files = 100; // 允许打开的最大文件数，可以根据实际情况调整
    //LevelDB 使用了内部的缓存来存储数据块。对于读操作较多的应用，可能会需要一个较大的缓存来提高读取性能。
    options.block_cache = leveldb::NewLRUCache(50 * 1048576); // 设置50MB的缓存
    //确定存储单元块的大小。较小的块可以提高读取效率，因为需要读取的无关数据更少。
    options.block_size = 4096; // 设置块大小为4KB
    //LevelDB 默认使用 Snappy 压缩来存储数据。如果磁盘空间不是问题，可以关闭它以提高写入性能。
    options.compression = leveldb::kNoCompression; // 关闭压缩以提高写入速度

    leveldb::Status status = leveldb::DB::Open(options, db_path, &db_);
    if (!status.ok()) {
      throw std::runtime_error("Unable to open/create database: " + db_path);
    }
  }

  ~LevelDBWrapper() {
    delete db_;
  }

  void Put(const std::string &key, const std::string &value) {
    leveldb::Status status = db_->Put(leveldb::WriteOptions(), key, value);
    if (!status.ok()) {
      throw std::runtime_error("Failed to put key: " + key);
    }
  }

  std::string Get(const std::string &key) {
    std::string value;
    leveldb::Status status = db_->Get(leveldb::ReadOptions(), key, &value);
    if (!status.ok()) {
      throw std::runtime_error("Failed to get key: " + key);
    }
    return value;
  }

  void Delete(const std::string &key) {
    leveldb::Status status = db_->Delete(leveldb::WriteOptions(), key);
    if (!status.ok()) {
      throw std::runtime_error("Failed to delete key: " + key);
    }
  }

  void WriteBatch(const std::vector<std::pair<std::string, std::string>> &updates) {
    leveldb::WriteBatch batch;
    for (const auto &update : updates) {
      batch.Put(update.first, update.second);
    }
    leveldb::Status status = db_->Write(leveldb::WriteOptions(), &batch);
    if (!status.ok()) {
      throw std::runtime_error("Failed to write batch updates");
    }
  }

 private:
  leveldb::DB *db_;
};

#endif //XUANOCPX_INCLUDE_LEVELDBWRAPPER_HPP_
