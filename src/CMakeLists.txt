
aux_source_directory(. DIR_SRCS)
add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME}
        ${LIBHV}
        ${LOGPP}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${LEVELDB_LIBRARY}
        pthread
)