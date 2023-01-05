cmake_minimum_required(VERSION 3.13)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()


project(grpc-proto)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

# gRPC
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG v1.49.2
)
FetchContent_MakeAvailable(gRPC)

# Protobuf
find_package(Protobuf REQUIRED)

# Create library
set(PROTO_FILES 
  ./dbpb/address.proto
  ./dbpb/addressbook.proto)

# Builds
include(./cmake/cpp.cmake)
