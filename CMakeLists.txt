cmake_minimum_required(VERSION 3.13)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()
# -----------------------------------------------------------------------------
# Basic setup
# -----------------------------------------------------------------------------
set(PROJECT_NAME csm) # client-server model
project(${PROJECT_NAME} LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-std=c++2b ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-std=c2x ${CMAKE_C_FLAGS}")

# Libs
add_subdirectory(qpg)
add_subdirectory(dbms)

# Client-server
add_subdirectory(proto)
add_subdirectory(arc)
add_subdirectory(client)
add_subdirectory(server)
