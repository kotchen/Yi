cmake_minimum_required(VERSION 3.10)
project(serialization)
find_package(Protobuf REQUIRED)
if (PROTOBUF_FOUND)
    message("protobuf found")
else ()
    message("protobuf not found")
endif()


file(GLOB proto_files proto/Request.proto proto/Message.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${proto_files})

message("PROTO_SRCS = ${PROTO_SRCS}")
message("PROTO_HDRS = ${PROTO_HDRS}")


aux_source_directory(${PROJECT_SOURCE_DIR} src_files)

SET(serialization_source ${src_files} PARENT_SCOPE)

# 将PROTO_SRCS生成静态库nart_proto.a文件
add_library(serialization STATIC ${PROTO_SRCS} Parser.h)

target_include_directories(serialization
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${PROTOBUF_INCLUDE_DIRS})


