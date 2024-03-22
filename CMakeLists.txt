cmake_minimum_required(VERSION 3.10)
project(xuanOcpx)

add_executable(xuanOcpx main.cpp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-pthread) # 推荐使用这种方式添加编译选项

# 查找Threads库
find_package(Threads REQUIRED)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# libhv
find_library(LIBHV NAMES hv PATHS /usr/local/lib REQUIRED)

# logpp
find_library(LOGPP NAMES logpp PATHS /usr/local/lib REQUIRED)

include_directories(
        /usr/local/include
        ${CMAKE_SOURCE_DIR}/include
        ${Nlohmann_INCLUDE_DIRS}
)

# libhv
find_library(LIBHV hv PATHS /usr/local/lib) # 推荐使用find_library来查找库
message(STATUS "libhv: ${LIBHV}")

aux_source_directory(src DIR_SRCS)
add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME}
        ${LIBHV}
        ${LOGPP}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
)