project(client LANGUAGES C CXX)

set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

find_package(Boost 1.80.0 REQUIRED COMPONENTS 
  program_options)

# Client executable
add_executable(${PROJECT_EXEC_NAME} 
  ./src/main.cpp)

target_link_libraries(${PROJECT_EXEC_NAME} PRIVATE 
  qpg
  proto::grpc_cpp
  arc::qpg::proto

  ${Boost_LIBRARIES}
)
