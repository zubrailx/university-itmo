cmake_minimum_required(VERSION 3.13)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()
# -----------------------------------------------------------------------------
# Basic setup
# -----------------------------------------------------------------------------
project(qpg 
  DESCRIPTION "Query plan generator"
  LANGUAGES C CXX)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Library
add_subdirectory(src)

# Executable
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(apps)
endif()
