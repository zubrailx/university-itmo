# basic project info
cmake_minimum_required(VERSION 3.12)

set(PROJECT 03-any)
set(LIB_NAME 03-any)
set(EXEC_NAME any_exec)

project(${PROJECT} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CONFIGURATION_TYPES Debug Sanitized Release)

# create a static library
set(SRCS
  src/Any.cpp
)
set(HDRS
  include/Any.h
)

add_library(${LIB_NAME} STATIC ${SRCS})
target_include_directories(${LIB_NAME} PUBLIC
        include
)

# compilation/linking
target_compile_options(${LIB_NAME} PUBLIC
        # -Wall -Werror -g
        -Wall -g
        "$<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak>"
)
target_link_options(${LIB_NAME} PUBLIC
        # -Wall -Werror -g
        -Wall -g
        "$<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak>"
)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3")


# if main.cpp is present
if (EXISTS "${PROJECT_SOURCE_DIR}/src/main.cpp")
    add_executable(${EXEC_NAME} src/main.cpp)
    target_link_libraries(${EXEC_NAME} PRIVATE ${LIB_NAME})
endif()


# testing
enable_testing()

set(TESTS
        test/test.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(any_test ${TESTS})
target_link_libraries(any_test ${LIB_NAME} gtest_main)

include(GoogleTest)
gtest_discover_tests(any_test)


# clang-format
add_custom_target(clang-format
        COMMAND
        clang-format
        -style=file
        -i
        ${SRCS} ${HDRS} ${TESTS}
        WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
        COMMENT
        "Running clang-format on sources..."
)
