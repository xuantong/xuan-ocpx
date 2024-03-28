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

# Hiredis
find_library(HIREDIS_LIBRARY NAMES hiredis PATHS /usr/lib/x86_64-linux-gnu)
if (NOT HIREDIS_LIBRARY)
    message(FATAL_ERROR "Hiredis library not found")
endif ()

# proto
find_package(Protobuf REQUIRED)
# 如果 Protobuf 找到了，包含 Protobuf 的 CMake 函数
if (PROTOBUF_FOUND)
    message(STATUS "Protobuf version : ${Protobuf_VERSION}")
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
else ()
    message(FATAL_ERROR "Cannot find Protobuf library.")
endif ()

# 设置 proto 文件
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/protos/*.proto")
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

message(STATUS "PROTO_FILES is ${PROTO_FILES}")

# 手动调用protoc编译器生成Proto源文件和头文件
foreach (PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME_WE ${PROTO_FILE} NAME_WE)
    set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME_WE}.pb.cc")
    set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME_WE}.pb.h")
    set(PROTOC_COMMAND "${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out ${CMAKE_CURRENT_BINARY_DIR} --proto_path ${CMAKE_CURRENT_SOURCE_DIR}/protos ${PROTO_FILE}")
    message(STATUS "Protoc Command: ${PROTOC_COMMAND}")

    add_custom_command(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND ${PROTOC_COMMAND}
            DEPENDS ${PROTO_FILE}
            COMMENT "Running C++ protocol buffer compiler on ${PROTO_FILE}"
            VERBATIM
    )
endforeach ()

include_directories(
        /usr/local/include
        /usr/include
        ${CMAKE_SOURCE_DIR}/include
        ${Nlohmann_INCLUDE_DIRS}
        ${LEVELDB_INCLUDE_PATH}
        ${Protobuf_INCLUDE_DIRS}
        # 包含生成的*.pb.h文件的目录
        ${CMAKE_CURRENT_BINARY_DIR}
)

# 包含子目录
add_subdirectory(src)
add_subdirectory(test)