project(server LANGUAGES C CXX)

set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

# Client executable
find_package(Boost 1.80.0 REQUIRED COMPONENTS 
  program_options)

add_executable(${PROJECT_EXEC_NAME} 
  ./src/main.cpp)

target_link_libraries(${PROJECT_EXEC_NAME} PRIVATE 
  dbms
  qpg 
  proto::grpc_cpp
  ${Boost_LIBRARIES}
)
