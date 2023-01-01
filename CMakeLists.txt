cmake_minimum_required(VERSION 3.13)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()
# -----------------------------------------------------------------------------
# Basic setup
# -----------------------------------------------------------------------------
set(PROJECT_NAME qpg)
project(${PROJECT_NAME} LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "-std=c++2b ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-std=c2x ${CMAKE_C_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIGURATION_TYPES Debug Released Sanitized)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(PROJECT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_GENERATED_BIN_DIR ${CMAKE_BINARY_DIR}/gen)
# -----------------------------------------------------------------------------
# Configurations
# -----------------------------------------------------------------------------
set(FETCHCONTENT_QUIET OFF)
enable_testing()

include(FetchContent)
# gRPC
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
        )

# bison/flex
find_package(FLEX)
find_package(BISON)
# -----------------------------------------------------------------------------
# Flex / bison targets
# -----------------------------------------------------------------------------
set(PARSER_DIR ${CMAKE_GENERATED_BIN_DIR}/parse)

set(LEXER_OUT ${PARSER_DIR}/lexer.c)
set(PARSER_OUT ${PARSER_DIR}/parser.c)

set(SRC_PARSE ${PROJECT_SRC_DIR}/parse)

flex_target(LEXER 
  "${SRC_PARSE}/lexer.l" 
  "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/lexer.h")

bison_target(PARSER 
  "${SRC_PARSE}/parser.y" 
  "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/parser.h")

add_flex_bison_dependency(LEXER PARSER)
# -----------------------------------------------------------------------------
# Library
# -----------------------------------------------------------------------------
set(PROJECT_LIB_NAME lib${PROJECT_NAME})

set(LIB_HDRS
)

set(LIB_SRCS
  src/main.c
)

add_library(${PROJECT_LIB_NAME} STATIC ${LIB_SRCS} ${LEXER_OUT} ${PARSER_OUT})
target_include_directories(${PROJECT_LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_LIB_NAME} PRIVATE ${PARSER_DIR})

target_compile_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
  $<$<CONFIG:RELEASED>:-O3>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)
target_link_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
  $<$<CONFIG:RELEASED>:-O3>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)
# -----------------------------------------------------------------------------
# Executable main.c
# -----------------------------------------------------------------------------
set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

set(EXEC_SRC ${PROJECT_SOURCE_DIR}/app/main.c)

if (EXISTS ${EXEC_SRC})
  add_executable(${PROJECT_EXEC_NAME} ${EXEC_SRC})
  target_link_libraries(${PROJECT_EXEC_NAME} ${PROJECT_LIB_NAME})
endif()
# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

set(TSTS

)

if (DEFINED TSTS)
	FetchContent_MakeAvailable(googletest)

  add_executable(${PROJECT_TEST_NAME} ${TSTS})
  target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME} gtest_main)

  add_test(NAME ${PROJECT_TEST_NAME} COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}> 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
# -----------------------------------------------------------------------------
# Formatting
# -----------------------------------------------------------------------------
add_custom_target(clang-format
        COMMAND
        clang-format
        -style=file
        -i
        ${LIB_SRCS} ${LIB_HDRS} ${EXEC_SRC} ${TSTS}
        WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
        COMMENT
        "Running clang-format on sources..."
)
