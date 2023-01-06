#include "dbpb/database.pb.h"
#include <boost/program_options.hpp>
#include <dbpb/database.grpc.pb.h>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include <iostream>

#include <qpg/qpg.hpp>

namespace po = boost::program_options;

constexpr int DEFAULT_PORT = 6543;

class DatabaseQueryService final : public DatabaseQuery::Service {
public:
  virtual ::grpc::Status PerformQuery(::grpc::ServerContext *context,
                                      const ::DatabaseRequest *request,
                                      ::DatabaseResponse *response) {
    std::cout << request->db_name() << std::endl;
    std::cout << request->command() << std::endl;
    return grpc::Status::OK;
  }
};

void run_server(const int port) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:" + std::to_string(port),
                           grpc::InsecureServerCredentials());

  DatabaseQueryService db_service;
  builder.RegisterService(&db_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Listening port: " << port << "." << std::endl;
  server->Wait();
}

int main(int argc, char *argv[]) {
  po::options_description desc("Client options");

  desc.add_options()("help,h", "Show help")("port,p", po::value<int>(),
                                            "Server port (default=6543)");

  po::variables_map vm;
  try {
    po::parsed_options parsed =
        po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);
    // Process command arguments
    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }
    const int port = !vm.count("port") ? DEFAULT_PORT : vm["port"].as<int>();

    run_server(port);

  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
  }
  return 0;
}
