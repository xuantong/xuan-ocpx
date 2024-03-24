cmake_minimum_required(VERSION 3.10)
project(xuanOcpx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-pthread) # 推荐使用这种方式添加编译选项

# level
set(LEVELDB_INCLUDE_PATH "/home/ubuntu/workspace/leveldb/include")
set(LEVELDB_LIBRARY "/home/ubuntu/workspace/leveldb/build/libleveldb.a")
message(STATUS "LEVELDB_LIBRARY :${LEVELDB_LIBRARY}")
message(STATUS "LEVELDB_INCLUDE_PATH :${LEVELDB_INCLUDE_PATH}")

# 查找Threads库
find_package(Threads REQUIRED)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# libhv
find_library(LIBHV NAMES hv PATHS /usr/local/lib REQUIRED)
message(STATUS "libhv: ${LIBHV}")

# logpp
find_library(LOGPP NAMES logpp PATHS /usr/local/lib REQUIRED)

include_directories(
        /usr/local/include
        ${CMAKE_SOURCE_DIR}/include
        ${Nlohmann_INCLUDE_DIRS}
        ${LEVELDB_INCLUDE_PATH}
)

# 包含子目录
add_subdirectory(src)
add_subdirectory(test)