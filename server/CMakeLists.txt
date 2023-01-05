project(server LANGUAGES C CXX)

set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

# Client executable
add_executable(${PROJECT_EXEC_NAME} 
  ./src/main.cpp)

target_link_libraries(${PROJECT_EXEC_NAME} PRIVATE 
  dbms
  proto::grpc_cpp
)
