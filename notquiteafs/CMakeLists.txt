cmake_minimum_required(VERSION 3.19)
project(basic_grpc)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

get_filename_component(notquiteafs_proto "./notquiteafs.proto" ABSOLUTE)
get_filename_component(notquiteafs_proto_path "${notquiteafs_proto}" PATH)

set(notquiteafs_proto_sources "${CMAKE_CURRENT_BINARY_DIR}/notquiteafs.pb.cc")
set(notquiteafs_proto_headers "${CMAKE_CURRENT_BINARY_DIR}/notquiteafs.pb.h")
set(notquiteafs_grpc_sources "${CMAKE_CURRENT_BINARY_DIR}/notquiteafs.grpc.pb.cc")
set(notquiteafs_grpc_headers "${CMAKE_CURRENT_BINARY_DIR}/notquiteafs.grpc.pb.h")
add_custom_command(
        OUTPUT "${notquiteafs_proto_sources}" "${notquiteafs_proto_headers}" "${notquiteafs_grpc_sources}" "${notquiteafs_grpc_headers}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}" --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${notquiteafs_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${notquiteafs_proto}"
        DEPENDS "${notquiteafs_proto}")

add_library(notquiteafs_grpc_proto
        ${notquiteafs_proto_sources}
        ${notquiteafs_proto_headers}
        ${notquiteafs_grpc_sources}
        ${notquiteafs_grpc_headers})
target_link_libraries(notquiteafs_grpc_proto
        ${_PROTOBUF_LIBPROTOBUF}
        ${_REFLECTION}
        ${_GRPC_GRPCPP})

add_subdirectory(client)
add_subdirectory(server)
