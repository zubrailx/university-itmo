add_library(arc_qpg_proto
  main.cpp
)
add_library(arc::qpg::proto ALIAS arc_qpg_proto)

target_link_libraries(arc_qpg_proto PRIVATE
  proto::grpc_cpp
  qpg
)

target_include_directories(arc_qpg_proto PUBLIC "../../include")
