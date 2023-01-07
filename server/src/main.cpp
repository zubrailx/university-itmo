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
  void SSAstSelect(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_SELECT);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstUpdate(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_UPDATE);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstDelete(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_DELETE);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstInsert(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_INSERT);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstCreate(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_CREATE);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstDrop(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;
    // header
    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_DROP);
    response.set_allocated_header(header_row);

    writer->Write(response);
  }
  void SSAstUnspecified(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse response;

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_UNSPECIFIED);
    response.set_allocated_header(header_row);
    grpc::WriteOptions options;

    options.set_last_message();
    writer->Write(response, options);
  }

  void SSAstSelectType(grpc::ServerWriter<DatabaseResponse> *writer, Ast *root) {
    switch (root->getType()) {
    case AstType::SELECT:
      return SSAstSelect(writer, root);
    case AstType::UPDATE:
      return SSAstUpdate(writer, root);
    case AstType::DELETE:
      return SSAstDelete(writer, root);
    case AstType::INSERT:
      return SSAstInsert(writer, root);
    case AstType::DROP:
      return SSAstDrop(writer, root);
    case AstType::CREATE:
      return SSAstCreate(writer, root);
    default:
      return SSAstUnspecified(writer, root);
    }
  }

public:
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
      SSAstSelectType(writer, it.get());
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
