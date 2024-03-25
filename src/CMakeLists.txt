# 搜集所有的源文件  
aux_source_directory(. DIR_SRCS)

# 将源文件编译成静态库  
add_library(xuanOcpx_core STATIC ${DIR_SRCS})

# 为静态库链接所需要的库  
target_link_libraries(xuanOcpx_core
        ${LIBHV}
        ${LOGPP}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${LEVELDB_LIBRARY}
)

# 创建可执行文件  
add_executable(${PROJECT_NAME} main.cpp)

# 将核心静态库链接到可执行文件  
target_link_libraries(${PROJECT_NAME}
        xuanOcpx_core
        pthread
)