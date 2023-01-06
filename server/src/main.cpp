#include <boost/program_options.hpp>

#include <dbpb/database.grpc.pb.h>
#include <dbpb/database.pb.h>
#include <grpcpp/server_builder.h>

#include <iostream>

#include <qpg/qpg.hpp>

namespace po = boost::program_options;
using namespace dbpb;

class DatabaseQueryService final : public DatabaseQuery::Service {
private:
  void ProcessAstSelect(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_SELECT);
  }
  void ProcessAstUpdate(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_UPDATE);
  }
  void ProcessAstDelete(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_DELETE);
  }
  void ProcessAstInsert(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_INSERT);
  }
  void ProcessAstCreate(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_CREATE);
  }
  void ProcessAstDrop(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_DROP);
  }
  void ProcessAstUnspecified(DatabaseResponse &response, const Ast *root) {
    response.set_query_type(QueryType::QUERY_TYPE_UNSPECIFIED);
  }

  void ProcessAstSingle(DatabaseResponse &response, Ast *root) {
    switch (root->getType()) {
    case AstType::SELECT:
      return ProcessAstSelect(response, root);
    case AstType::UPDATE:
      return ProcessAstUpdate(response, root);
    case AstType::DELETE:
      return ProcessAstDelete(response, root);
    case AstType::INSERT:
      return ProcessAstInsert(response, root);
    case AstType::DROP:
      return ProcessAstDrop(response, root);
    case AstType::CREATE:
      return ProcessAstCreate(response, root);
    default:
      return ProcessAstUnspecified(response, root);
    }
  }

public:
  ::grpc::Status PerformQuery(::grpc::ServerContext *context,
                              const ::DatabaseRequest *request,
                              ::DatabaseResponse *response) override {
    AstWrapper astw;
    int code = parse_command(request->command(), astw);
    if (code) {
      std::cout << "ERROR: " << astw.err_msg << std::endl;
    } else {
      std::cout << astw.list->repr() << std::endl;
    }
    return grpc::Status::OK;
  }

  grpc::Status PerformQuerySS(grpc::ServerContext *context,
                              const DatabaseRequest *request,
                              grpc::ServerWriter<DatabaseResponse> *writer) override {
    AstWrapper astw;
    int code = parse_command(request->command(), astw);

    if (code) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, astw.err_msg);
    }

    const auto *astList = astw.list->getList();
    for (const auto &it : *astList) {
      DatabaseResponse resp;
      ProcessAstSingle(resp, it.get());
      writer->Write(resp);
    }
    return grpc::Status::OK;
  }
};

// Run server
void run_server(const std::string &address, const int port) {
  grpc::ServerBuilder builder;
  std::string uri = address + ":" + std::to_string(port);
  builder.AddListeningPort(uri, grpc::InsecureServerCredentials());

  DatabaseQueryService db_service;
  builder.RegisterService(&db_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Listening uri: " << uri << "." << std::endl;
  server->Wait();
}

// Process command args and start server
int main(int argc, char *argv[]) {
  po::options_description desc("Client options");

  desc.add_options()
    ("help,h", "Show help")
    ("port,p", po::value<int>()->default_value(6543), "Server port")
    ("address,a", po::value<std::string>()->default_value("0.0.0.0"), "Listened address")
    ;

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

    run_server(address, port);

  } catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    std::cout << desc << std::endl;
  }
  return 0;
}
