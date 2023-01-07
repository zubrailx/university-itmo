#include <boost/program_options.hpp>
#include <fmt/core.h>

#include <dbpb/database.grpc.pb.h>
#include <dbpb/database.pb.h>
#include <grpcpp/server_builder.h>

#include <iostream>

extern "C" {
#include <dbms/database.h>
#include <dbms/dto_table.h>
#include <dbms/schema.h>
}
#include <qpg/qpg.hpp>

#include "converters.hpp"

namespace po = boost::program_options;
using namespace dbpb;

class DatabaseQueryService final : public DatabaseQuery::Service {
private:
  dbms *OpenDatabase(const std::string db_name, DatabaseResponse &resp) {
    dbms *dbms = database_open(db_name.c_str(), false);

    if (dbms == nullptr) {
      auto *err_status = new ErrorStatus();
      err_status->set_message(fmt::format("Cannot open database {}", db_name));
      resp.set_allocated_err_status(err_status);
    }
    return dbms;
  }

  void SSAstSelect(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_SELECT);
    resp.set_allocated_header(header_row);

    writer->Write(resp);
  }

  void SSAstUpdate(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_UPDATE);
    resp.set_allocated_header(header_row);

    writer->Write(resp);
  }

  void SSAstDelete(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_DELETE);
    resp.set_allocated_header(header_row);

    writer->Write(resp);
  }

  void SSAstInsert(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_INSERT);
    resp.set_allocated_header(header_row);

    writer->Write(resp);
  }

  void SSAstCreate(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_CREATE);

    const auto *create = (const AstCreate *)root;

    auto *dto_table = dto_table_construct(create->m_table.c_str());

    for (const auto &col : create->m_collist->m_lst) {
      auto tct = toTableColumnType(col->m_type);
      std::string colname = col->m_column->m_column;
      dto_table_add_column(dto_table, colname.c_str(), tct, {});
      // append to output message
      auto header_column = header_row->add_columns();
      header_column->set_column_name(colname);
      header_column->set_column_type(toString(tct));
    }

    bool res = table_create(dbms, dto_table);
    if (!res) {
      auto *err_status = new ErrorStatus();
      err_status->set_message(fmt::format("Table {} already exists", create->m_table));
      resp.set_allocated_err_status(err_status);
    }

    dto_table_destruct(&dto_table);
    database_close(&dbms, false);

    resp.set_allocated_header(header_row);
    writer->Write(resp);
  }

  void SSAstDrop(grpc::ServerWriter<DatabaseResponse> *writer,
                 const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_DROP);

    const auto *drop = (const AstDrop *)root;
    bool res = table_drop(dbms, drop->m_table.c_str());
    if (!res) {
      auto *err_status = new ErrorStatus();
      err_status->set_message(fmt::format("Table {} does not exists", drop->m_table));
      resp.set_allocated_err_status(err_status);
    }

    database_close(&dbms, false);

    resp.set_allocated_header(header_row);
    writer->Write(resp);
  }

  void SSAstUnspecified(grpc::ServerWriter<DatabaseResponse> *writer, const Ast *root) {
    DatabaseResponse resp;

    auto *header_row = new DatabaseHeaderRow();
    header_row->set_query_type(QueryType::QUERY_TYPE_UNSPECIFIED);
    resp.set_allocated_header(header_row);
    grpc::WriteOptions options;

    writer->Write(resp, options);
  }

  void SSAstSelectType(grpc::ServerWriter<DatabaseResponse> *writer,
                       const DatabaseRequest *req, Ast *root) {
    switch (root->getType()) {
    case AstType::SELECT:
      return SSAstSelect(writer, req, root);
    case AstType::UPDATE:
      return SSAstUpdate(writer, req, root);
    case AstType::DELETE:
      return SSAstDelete(writer, req, root);
    case AstType::INSERT:
      return SSAstInsert(writer, req, root);
    case AstType::DROP:
      return SSAstDrop(writer, req, root);
    case AstType::CREATE:
      return SSAstCreate(writer, req, root);
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

    for (const auto &ast : astw.list->m_lst) {
      std::cout << ast->repr() << std::endl;
      SSAstSelectType(writer, request, ast.get());
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

  desc.add_options()("help,h", "Show help")(
      "port,p", po::value<int>()->default_value(6543),
      "Server port")("address,a", po::value<std::string>()->default_value("0.0.0.0"),
                     "Listened address");

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
