set(PROJECT_EXEC_NAME ${PROJECT_NAME}_exec)

add_executable(${PROJECT_EXEC_NAME} 
  main.cpp)
target_link_libraries(${PROJECT_EXEC_NAME} qpg)
