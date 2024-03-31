# 搜集所有的源文件  
aux_source_directory(. DIR_SRCS)

# 将Proto源文件添加到DIR_SRCS变量中
list(APPEND DIR_SRCS ${PROTO_SRCS})

# 假设你的PROTO_SRCS变量包含了所有protobuf生成的源文件
message(STATUS "Protobuf sources: ${PROTO_HDRS}")

# 将源文件编译成静态库
add_library(xuanOcpx_core ${DIR_SRCS} ${PROTO_HDRS})

# 为静态库链接所需要的库
target_link_libraries(xuanOcpx_core
        ${LIBHV}
        ${LOGPP}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${LEVELDB_LIBRARY}
        ${HIREDIS_LIBRARY}
        ${PROTOBUF_LIBRARIES}
)

# 创建可执行文件  
add_executable(${PROJECT_NAME} main.cpp)

# 将核心静态库链接到可执行文件  
target_link_libraries(${PROJECT_NAME}
        xuanOcpx_core
        pthread
)