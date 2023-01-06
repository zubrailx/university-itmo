set(PROJECT_LIB_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRCS 
  CONFIGURE_DEPENDS 
  ./*.c 
  ./*.cpp)

# Bison/Flex
find_package(FLEX)
find_package(BISON)

set(PARSER_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${PARSER_DIR})

set(LEXER_OUT ${PARSER_DIR}/lexer.cpp)
set(PARSER_OUT ${PARSER_DIR}/parser.cpp)

flex_target(LEXER 
  "./parse.l" 
  "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/lexer.hpp")

bison_target(PARSER 
  "./parse.y" 
  "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/parser.hpp")

# Add library with flex/bison
add_library(${PROJECT_LIB_NAME} STATIC ${SRCS} ${LEXER_OUT} ${PARSER_OUT})
target_include_directories(${PROJECT_LIB_NAME} PUBLIC "../include")
target_include_directories(${PROJECT_LIB_NAME} PRIVATE ${PARSER_DIR})
target_link_libraries(${PROJECT_LIB_NAME}
  )

target_compile_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
)

target_link_options(${PROJECT_LIB_NAME} PUBLIC
  -Wall
  $<$<CONFIG:DEBUG>:-g -O0>
)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
