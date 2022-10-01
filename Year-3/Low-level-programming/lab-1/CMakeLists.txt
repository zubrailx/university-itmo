cmake_minimum_required(VERSION 3.13)
# -----------------------------------------------------------------------------
# Basic setup
# -----------------------------------------------------------------------------
set(PROJECT_NAME dbms)
set(PROJECT_LIB_NAME ${PROJECT_NAME})
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
project(${PROJECT_NAME} LANGUAGES C CXX)

# set latest standard
# set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-std=c++2b ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-std=c2x ${CMAKE_C_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
add_compile_options(
  -Wall
  # -Wextra
  # -Wconversion
  # -Wsign-conversion
  $<$<CONFIG:DEBUG>:-g>
  $<$<CONFIG:DEBUG>:-Og>
  $<$<CONFIG:RELEASE>:-O3>
)

add_link_options()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/example)

file(GLOB_RECURSE SRCS
  CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.c)

file(GLOB_RECURSE HDRS
  CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/include/*.h)

file(GLOB_RECURSE TSTS
  CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/test/*.cpp)
# -----------------------------------------------------------------------------
# Configurations
# -----------------------------------------------------------------------------
  # LIBRARIES
  set(FETCHCONTENT_QUIET OFF)
  include(FetchContent)
  # gRPC
  FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
  )
  FetchContent_MakeAvailable(googletest)
  # CREATE_LIBRARY
  add_library(${PROJECT_LIB_NAME} STATIC ${SRCS})
  target_include_directories(${PROJECT_LIB_NAME} PUBLIC include)

  # LINKING AND EXECUTABLES
  add_executable(${PROJECT_TEST_NAME} ${TSTS})
  target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME} gtest_main)

if (DEFINED PROGRAM_LINUX OR UNIX)
  # message(DEBUG ${UNIX})
  # message(STATUS "[info]: HELLO, UNIX user")
elseif (DEFINED PROGRAM_WINDOWS OR WIN32)
  # message(DEBUG ${WIN32})
  # message(STATUS "[info]: HELLO, WINDOWS user")
endif()

# Ignore CMake warning: not used variable
set(ignoreMe "${PROGRAM_LINUX}${PROGRAM_WINDOWS}")
# Remove configuration variables from cache
unset(PROGRAM_LINUX CACHE)
unset(PROGRAM_WINDOWS CACHE)
# -----------------------------------------------------------------------------
# Testing
# -----------------------------------------------------------------------------
enable_testing()
add_test(${PROJECT_TEST_NAME} bin/${PROJECT_TEST_NAME})
# include(GoogleTest)
# gtest_discover_tests(${PROJECT_TEST_NAME})
# -----------------------------------------------------------------------------
# Formatting
# -----------------------------------------------------------------------------
# clang-format
add_custom_target(clang-format
        COMMAND
        clang-format
        -style=file
        -i
        ${SRCS} ${HDRS} ${TSTS}
        WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
        COMMENT
        "Running clang-format on sources..."
)
