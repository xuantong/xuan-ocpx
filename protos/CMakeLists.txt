file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto") # 注意这里的路径改动

# 为每个proto文件生成C++代码  
foreach (PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(PROTO_SRC "${PROTO_OUTPUT_PATH}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDR "${PROTO_OUTPUT_PATH}/${PROTO_NAME}.pb.h")
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE}) # 改动这里，使用变量PROTO_SRCS和PROTO_HDRS来收集源文件和头文件  
    add_library(${PROTO_NAME}_proto ${PROTO_SRCS} ${PROTO_HDRS})
    target_include_directories(${PROTO_NAME}_proto PUBLIC ${PROTO_OUTPUT_PATH})
endforeach () 