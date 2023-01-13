set(PROJECT_LIB_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ./*.c)

add_library(${PROJECT_LIB_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_LIB_NAME} PUBLIC "../include")

target_compile_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)

target_link_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
  $<$<CONFIG:SANITIZED>:-fsanitize=address,undefined,leak -g>
)

message(DEBUG ${CMAKE_CXX_FLAGS_RELEASE})

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

