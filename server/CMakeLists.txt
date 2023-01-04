cmake_minimum_required(VERSION 3.13)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# Basic setup
project(dbms 
  DESCRIPTION "Database Management System"
  LANGUAGES C CXX)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_CONFIGURATION_TYPES Debug Released Sanitized)

  include(CTest)
endif()

# External libraries
set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

# Library
add_subdirectory(src)

# Executable
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(apps)
endif()

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
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
endif()
