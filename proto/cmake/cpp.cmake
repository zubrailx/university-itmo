add_library(proto_grpc_cpp ${PROTO_FILES})
add_library(proto::grpc_cpp ALIAS proto_grpc_cpp)

target_link_libraries(proto_grpc_cpp PUBLIC
  libprotobuf
  grpc
  grpc++
)

set(PROTOC_CPP_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cpp)
file(MAKE_DIRECTORY ${PROTOC_CPP_OUT_DIR})

target_include_directories(proto_grpc_cpp PUBLIC ${PROTOC_CPP_OUT_DIR})

# Generate
find_program(_GRPC_CPP_PLUGIN_LOCATION grpc_cpp_plugin)

protobuf_generate(
  TARGET proto_grpc_cpp 
  LANGUAGE cpp
  PROTOC_OUT_DIR ${PROTOC_CPP_OUT_DIR}
)
protobuf_generate(TARGET proto_grpc_cpp LANGUAGE grpc 
  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
  PLUGIN "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_LOCATION}"
  PROTOC_OUT_DIR ${PROTOC_CPP_OUT_DIR}
)
