project(server LANGUAGES C CXX)

set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

# External libraries
find_package(Boost 1.80.0 REQUIRED COMPONENTS 
  program_options)

set(FETCHCONTENT_QUIET OFF)
include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

# Executable
add_executable(${PROJECT_EXEC_NAME} 
  ./src/converters.cpp
  ./src/main.cpp
)

target_link_libraries(${PROJECT_EXEC_NAME} PRIVATE 
  dbms
  qpg
  proto::grpc_cpp
  arc::qpg::proto

  fmt::fmt
  ${Boost_LIBRARIES}
)
