
## 启用测试功能，这允许使用 `add_test()` 命令。
enable_testing()

# 查找并加载GTest库的CMake配置信息。这是必需的，以便链接GTest库。
find_package(GTest REQUIRED)

# 包含GTest的头文件目录，这样测试代码就可以找到GTest的头文件。
include_directories(${GTEST_INCLUDE_DIRS})

# 添加本项目的头文件目录，这允许测试代码包含项目中的头文件。
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROTO_OUTPUT_PATH}
)

# 创建一个名为 main_test 的可执行文件，包含以下两个源文件。
add_executable(main_test
        main_test.cpp
        macro_variable_util_test.cpp
        leveldb_wrapper_test.cpp
        string_utils_test.cpp
        enum_util_test.cpp
        cache_manager_test.cpp
        hash_utils_test.cpp
        redis_client_test.cpp
        protos_test.cpp
)

# 将 main_test 可执行文件与我们的数学函数库以及预编译的GTest和GMock库进行链接。
target_link_libraries(main_test
        xuanOcpx_core
        GTest::GTest
        GTest::Main
        OpenSSL::SSL
        ${LEVELDB_LIBRARY}
        ${PROTOBUF_LIBRARIES}
        tracer_callback_proto
        tracer_req_proto
        tracer_config_proto
        pthread
)

# 添加一个测试用例，名为 math_functions_test。
add_test(NAME main
        COMMAND main_test)
