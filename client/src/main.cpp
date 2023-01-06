#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <dbpb/database.grpc.pb.h>

#include <iostream>
#include <string>

namespace po = boost::program_options;

constexpr int DEFAULT_PORT = 6543;
const std::string DEFAULT_DBFILE = "tmp/db.bin";

void run_client(const std::string &dbfile, const int port) {
  std::string buf;
  std::string line;

  // Create channel and attach
  auto channel = grpc::CreateChannel("localhost:" + std::to_string(DEFAULT_PORT),
                                     grpc::InsecureChannelCredentials());

  std::unique_ptr<DatabaseQuery::Stub> stub = DatabaseQuery::NewStub(channel);

  DatabaseRequest request;
  DatabaseResponse response;

  std::cout << "Created channel on port: " << port << "." << std::endl;
  std::cout << "Working with db: " << dbfile << "." << std::endl;

  std::cout << "> ";
  while (getline(std::cin, line)) {
    // Check if command end
    std::string line_cp = boost::trim_right_copy(line);
    size_t cp_size = line_cp.size();

    bool query_ended = cp_size && line_cp[cp_size - 1] == ';';

    // Append trimmed line if ended
    if (query_ended) {
      buf.append(line_cp);
    } else {
      buf.append(line);
      buf.append("\n");
    }

    if (query_ended) {
      // append trimmed line
      grpc::ClientContext context;

      request.set_db_name(dbfile);
      request.set_command(buf);

      grpc::Status status = stub->PerformQuery(&context, request, &response);

      if (!status.ok()) {
        std::cout << "ERROR(code=" << status.error_code()
                  << "): " << status.error_message() << std::endl;
      }
      // New iteration
      buf.clear();
      std::cout << "> ";
    }
  }
}

int main(int argc, char *argv[]) {

  po::options_description desc("Client options");

  desc.add_options()("help,h", "Show help")("port,p", po::value<int>(),
                                            "Server port (default=6543)")(
      "database,d", po::value<std::string>(), "Database file (default=tmp/db.bin)");

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
    const std::string dbfile =
        !vm.count("database") ? DEFAULT_DBFILE : vm["database"].as<std::string>();

    run_client(dbfile, port);

  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
  }
  return 0;
}
