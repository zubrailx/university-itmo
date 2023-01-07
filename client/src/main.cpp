#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <dbpb/database.grpc.pb.h>
#include <google/protobuf/util/json_util.h>

#include <iostream>
#include <string>

namespace po = boost::program_options;
using namespace dbpb;

void process_status(const grpc::Status &status) {
  if (!status.ok()) {
    std::cout << "ERROR(code=" << status.error_code() << "): " << status.error_message()
              << std::endl;
  }
}

grpc::Status perform_single(std::unique_ptr<DatabaseQuery::Stub> &stub,
                            const DatabaseRequest &request,
                            DatabaseResponse &response) {
  grpc::ClientContext context;
  grpc::Status status = stub->PerformQuery(&context, request, &response);

  return status;
}

grpc::Status perform_server_stream(std::unique_ptr<DatabaseQuery::Stub> &stub,
                                   const DatabaseRequest &request,
                                   DatabaseResponse &response) {
  grpc::ClientContext context;
  std::unique_ptr<grpc::ClientReader<DatabaseResponse>> reader(
      stub->PerformQuerySS(&context, request));

  google::protobuf::util::JsonOptions json_opts;
  json_opts.add_whitespace = true;

  bool last_header = false;

  while (reader->Read(&response)) {
    if (response.has_err_status()) {
      std::cout << "Error: ";
      std::cout << response.err_status().message() << std::endl;
    } else {
      if (response.has_header()) {
        std::cout << "TABLE HEADER:" << std::endl;

        DatabaseHeaderRow header_row = response.header();
        std::string bufout;
        google::protobuf::util::MessageToJsonString(header_row, &bufout, json_opts);
        std::cout << bufout;

        last_header = true;
      } else {
        if (last_header) {
          std::cout << "TABLE BODY:" << std::endl;
          last_header = false;
        }
        DatabaseBodyRow body_row = response.body();
        std::string bufout;
        google::protobuf::util::MessageToJsonString(body_row, &bufout, json_opts);
        std::cout << bufout;
      }
    }
  }

  return reader->Finish();
}

void run_client(const std::string &dbfile, const std::string &address, const int port) {
  std::string buf;
  std::string line;

  // Create channel and attach
  std::string target = address + ":" + std::to_string(port);
  auto channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());

  std::unique_ptr<DatabaseQuery::Stub> stub = DatabaseQuery::NewStub(channel);
  std::cout << "Created channel on target: " << target << "." << std::endl;
  std::cout << "Working with db: " << dbfile << "." << std::endl;

  DatabaseRequest request;
  request.set_db_name(dbfile);
  DatabaseResponse response;

  std::cout << "> ";
  while (getline(std::cin, line)) {
    // Check if command end
    std::string line_cp = boost::trim_right_copy(line);
    size_t cp_size = line_cp.size();

    // Append trimmed line if ended
    bool query_ended = cp_size && line_cp[cp_size - 1] == ';';
    if (query_ended) {
      buf.append(line_cp);
    } else {
      buf.append(line);
      buf.append("\n");
    }

    // Perform request
    if (query_ended) {
      request.set_command(buf);

      // auto status = perform_single(stub, request, response);
      auto status = perform_server_stream(stub, request, response);

      process_status(status);
    }
    // New iteration
    buf.clear();
    std::cout << "> ";
  }
}

int main(int argc, char *argv[]) {

  po::options_description desc("Client options");

  desc.add_options()("help,h", "Show help")(
      "address,a", po::value<std::string>()->default_value("localhost"),
      "Server address")("port,p", po::value<int>()->default_value(6543), "Server port")(
      "database,d", po::value<std::string>()->default_value(".tmp-db.bin"),
      "Database file");

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
    const int port = vm["port"].as<int>();
    const std::string address = vm["address"].as<std::string>();
    const std::string dbfile = vm["database"].as<std::string>();

    run_client(dbfile, address, port);

  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
  }
  return 0;
}
