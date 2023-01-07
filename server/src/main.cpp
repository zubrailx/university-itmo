#include <boost/program_options.hpp>
#include <fmt/core.h>

#include <dbpb/database.grpc.pb.h>
#include <dbpb/database.pb.h>
#include <grpcpp/server_builder.h>

#include <iostream>

extern "C" {
#include <dbms/database.h>
#include <dbms/dto_row.h>
#include <dbms/dto_table.h>
#include <dbms/plan.h>
#include <dbms/plan_filter.h>
#include <dbms/plan_funcs.h>
#include <dbms/schema.h>
#include <dbms/table.h>
}
#include <qpg/qpg.hpp>

#include "converters.hpp"

namespace po = boost::program_options;
using namespace dbpb;

class DatabaseQueryService final : public DatabaseQuery::Service {
private:
  dbms *OpenDatabase(const std::string &db_name, DatabaseResponse &resp) {
    dbms *dbms = database_open(db_name.c_str(), false);

    if (dbms == nullptr) {
      auto *err_status = resp.mutable_err_status();
      err_status->set_message(fmt::format("Cannot open database {}", db_name));
    }
    return dbms;
  }

  // idxs in ast_idx represent idxs of columns in actual table
  void MapTableColumnIdxs(std::vector<int> &ast_idxs, dto_table *dto_header,
                          const std::list<std::unique_ptr<AstColumn>> &collist) {
    size_t ast_idx = 0;
    for (const auto &ast_col : collist) {
      size_t dto_idx = 0;
      for (const auto *column = dto_header->first; column != nullptr;
           column = column->next) {
        if (strcmp(ast_col->m_column.c_str(), column->name) == 0) {
          ast_idxs[ast_idx] = static_cast<int>(dto_idx);
        }
        ++dto_idx;
      }
      ++ast_idx;
    }
  }

  void FillResponseTableHeader(DatabaseResponse &resp, dto_table *dto_header) {
    auto *header_row = resp.mutable_header();
    for (const auto *col = dto_header->first; col != nullptr; col = col->next) {
      DatabaseHeaderColumn *row_column = header_row->add_columns();
      row_column->set_column_name(std::string(col->name));
      row_column->set_column_type(toString(col->type));
    }
  }

  bool CheckTableColumnIdxs(DatabaseResponse &resp, std::vector<int> &ast_idxs,
                            const AstColumnList *collist) {
    size_t col_idx = 0;
    for (const auto &col : collist->m_lst) {
      if (ast_idxs[col_idx] < 0) {
        auto *err_status = resp.mutable_err_status();
        err_status->set_message(
            fmt::format("Column {} is not present in table", col->m_column));
        return false;
      }
    }
    return true;
  }

  bool CheckTableColumnTypes(DatabaseResponse &resp, dto_table *dto_header,
                             std::vector<int> &ast_idxs,
                             const AstList<AstValue> *vallist) {
    size_t ast_idx = 0;
    for (const auto &ast_val : vallist->m_lst) {

      int i = 0;
      auto *dto_col_cur = dto_header->first;
      while (i != ast_idxs[ast_idx]) {
        dto_col_cur = dto_col_cur->next;
        assert(dto_col_cur != nullptr);
        ++i;
      }

      auto tct = toTableColumnType(ast_val->m_dtype);
      if (tct != dto_col_cur->type) {
        auto *err_status = resp.mutable_err_status();
        err_status->set_message(
            fmt::format("Column {} has different type", dto_col_cur->name));
        return false;
      }
      ++ast_idx;
    }
    return true;
  }

  void GetTableHeaderWithAstMap(std::vector<int> &ast_idxs,
                                const std::string &table_name, struct dbms *dbms,
                                const AstColumnList *collist,
                                const AstList<AstValue> *vallist,
                                DatabaseResponse &resp) {
    auto *dto_header = table_construct_header(dbms, table_name.c_str());
    // Map database header
    if (!dto_header) {
      auto *err_status = resp.mutable_err_status();
      err_status->set_message(fmt::format("Table {} does not exist", table_name));
    } else {
      size_t col_size;

      if (collist->do_all) {
        col_size = dto_table_column_cnt(dto_header);
      } else {
        col_size = collist->m_lst.size();
      }
      // resize array and init with -1
      ast_idxs.resize(col_size);
      for (size_t i = 0; i < col_size; ++i) {
        ast_idxs[i] = -1;
      }
      // if do_all flag in collist is present
      if (collist->do_all) {
        for (int i = 0; i < (int)col_size; ++i) {
          ast_idxs[i] = i;
        }
      } else {
        MapTableColumnIdxs(ast_idxs, dto_header, collist->m_lst);
        // check for validity
        if (!CheckTableColumnIdxs(resp, ast_idxs, collist)) {
          goto fill_response;
        }
      }
      CheckTableColumnTypes(resp, dto_header, ast_idxs, vallist);
    fill_response:
      FillResponseTableHeader(resp, dto_header);
    }
    dto_table_destruct(&dto_header);
  }

  void PreprocAstColumnList(DatabaseResponse &resp, const AstColumnList *ast_clist) {
    if (ast_clist->m_lst.size()) {
      auto *err_status = resp.mutable_err_status();
      err_status->set_message("Projection in select is not supported");
    }
  }

  void StatusAstUnexpected(const Ast *ast) {
    std::cout << fmt::format("Unexpected ast type {}", (int)ast->getType())
              << std::endl;
    assert(0);
  }

  plan *ParseAsTableSource(DatabaseResponse &resp, const Ast *ast, dbms *dbms) {
    plan *out = nullptr;
    switch (ast->getType()) {
    case AstType::TABLE: {
      const auto *table = (AstTable *)ast;
      auto *plan_source = plan_source_construct(table->m_name.c_str(), dbms);
      if (table->m_has_alias) {
        out = (plan *)plan_select_construct_move(plan_source, table->m_alias.c_str());
      }
      break;
    }
    case AstType::SUBQUERY: {
      const auto *subq = (AstSubquery *)ast;
      auto *select = ParseAsSelect(resp, subq->m_query.get(), dbms);
      out = (plan *)plan_select_construct_move(select, subq->m_alias.c_str());
      break;
    }
    default:
      StatusAstUnexpected(ast);
    }
    return out;
  }

  plan *ParseAsTableRef(DatabaseResponse &resp, const Ast *ast, dbms *dbms) {
    plan *out = nullptr;
    switch (ast->getType()) {
    case AstType::TABLE:
    case AstType::SUBQUERY: {
      out = ParseAsTableSource(resp, ast, dbms);
      break;
    }
    case AstType::JOIN: {
      const auto *join = (AstJoin *)ast;
      auto *left = ParseAsTableRef(resp, join->m_lsv.get(), dbms);
      auto *right = ParseAsTableSource(resp, join->m_rsv.get(), dbms);
      out = (plan *)plan_cross_join_construct_move(left, right);
      break;
    }
    default:
      StatusAstUnexpected(ast);
    }
    return out;
  }

  fast *ParseAsStatement(DatabaseResponse &resp, const AstStatement *ast, dbms *dbms) {
    fast *out = nullptr;

    switch (ast->m_stype) {
    case StatementType::CONST: {
      auto *st_const = (AstStatementConst *)ast;
      auto tct = toTableColumnType(st_const->m_res->m_dtype);
      out = (fast *)fast_const_construct(tct, getAstValuePtr(st_const->m_res.get()),
                                         dbms);
      break;
    }
    case StatementType::COLUMN: {
      auto *st_column = (AstStatementColumn *)ast;
      const char *table = st_column->m_col->m_table.c_str();
      const char *column = st_column->m_col->m_column.c_str();
      out = (fast *)fast_column_construct(table, column, dbms);
      break;
    }
    case StatementType::UNARY: {
      auto *st_unary = (AstStatementUnary *)ast;
      auto *parent = ParseAsStatement(resp, st_unary->m_operand.get(), dbms);
      auto *func = toDbmsFunc(st_unary->m_op, parent->res_type);
      out = (fast *)fast_unop_construct((void *)parent, func, dbms);
      break;
    }
    case StatementType::BINARY: {
      auto *binary = (AstStatementBinary *)ast;
      auto *left = ParseAsStatement(resp, binary->m_left.get(), dbms);
      auto *right = ParseAsStatement(resp, binary->m_right.get(), dbms);
      auto *func = toDbmsFunc(binary->m_op, left->res_type, right->res_type);
      out = (fast *)fast_binop_construct(left, right, func, dbms);
      break;
    }
    default:
      StatusAstUnexpected(ast);
    }
    return out;
  }

  // NOTE: ParseAsSelect can return select or filter
  plan *ParseAsSelect(DatabaseResponse &resp, const AstSelect *ast, dbms *dbms) {
    const auto *ast_clist = ast->m_collist.get();
    PreprocAstColumnList(resp, ast_clist);

    plan *out = ParseAsTableRef(resp, ast->m_table_ref.get(), dbms);

    if (ast->m_has_cond) {
      fast *filter = ParseAsStatement(resp, ast->m_cond.get(), dbms);
      out = (plan *)plan_filter_construct_move(out, filter);
    }
    return out;
  }

  plan_select *ParseAsSelectWrapper(DatabaseResponse &resp, const AstSelect *ast,
                                    dbms *dbms) {
    auto *out = ParseAsSelect(resp, ast, dbms);
    if (out->type == PLAN_TYPE_SELECT) {
      return (plan_select *)out;
    } else {
      return plan_select_construct_move(out, "");
    }
  }

  void FillResponseTableHeader(DatabaseResponse &resp, const plan_table_info *pti,
                               dbms *dbms) {
    auto *header = resp.mutable_header();
  }

  void FillResponseTableBodyRow(DatabaseResponse &resp, const plan_table_info *pti,
                                const tp_tuple *tuple, dbms *dbms) {
    auto *body_row = resp.mutable_body();
  }

  // SELECT
  void SSAstSelect(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }
    // NOTE: projection is not supported
    plan_select *select = ParseAsSelectWrapper(resp, (const AstSelect *)root, dbms);

    size_t pti_size;
    const auto *pti = select->get_info(select, &pti_size);
    assert(pti_size == 1);

    auto *header_row = resp.mutable_header();
    header_row->set_query_type(QueryType::QUERY_TYPE_SELECT);
    FillResponseTableHeader(resp, pti, dbms);
    writer->Write(resp);

    // create new response for body
    resp = DatabaseResponse();

    select->start(select);
    while (!select->end(select)) {
      const tp_tuple *tuple = select->get(select)[0];
      FillResponseTableBodyRow(resp, pti, tuple, dbms);
      writer->Write(resp);
      select->next(select);
    }
    select->destruct(select);
  }

  plan_update *ParseAsUpdate(DatabaseResponse &resp, const AstUpdate *ast, dbms *dbms) {
    plan_update *out = nullptr;
    std::cout << "NOT IMPLEMENTED " << __PRETTY_FUNCTION__ << std::endl;
    return out;
  }

  // UPDATE
  void SSAstUpdate(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    plan_update *update = ParseAsUpdate(resp, (const AstUpdate *)root, dbms);

    size_t pti_size;
    const auto *pti = update->get_info(update, &pti_size);

    update->start(update);
    while (!update->end(update)) {
      update->next(update);
    }
    update->destruct(update);

    auto *header_row = resp.mutable_header();
    header_row->set_query_type(QueryType::QUERY_TYPE_UPDATE);
    FillResponseTableHeader(resp, pti, dbms);
    // write header at the end
    writer->Write(resp);
  }

  plan_delete *ParseAsDelete(DatabaseResponse &resp, const AstDelete *ast, dbms *dbms) {
    plan_delete *out = nullptr;
    std::cout << "NOT IMPLEMENTED " << __PRETTY_FUNCTION__ << std::endl;
    return out;
  }

  // DELETE
  void SSAstDelete(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    plan_delete *pdelete = ParseAsDelete(resp, (const AstDelete *)root, dbms);

    size_t pti_size;
    const auto *pti = pdelete->get_info(pdelete, &pti_size);

    pdelete->start(pdelete);
    while (!pdelete->end(pdelete)) {
      pdelete->next(pdelete);
    }
    pdelete->destruct(pdelete);

    auto *header_row = resp.mutable_header();
    header_row->set_query_type(QueryType::QUERY_TYPE_DELETE);
    FillResponseTableHeader(resp, pti, dbms);
    // write header at the end
    writer->Write(resp);
  }

  // INSERT
  void SSAstInsert(grpc::ServerWriter<DatabaseResponse> *writer,
                   const DatabaseRequest *req, const Ast *root) {
    DatabaseResponse resp;

    dbms *dbms = OpenDatabase(req->db_name(), resp);
    if (dbms == nullptr) {
      writer->Write(resp);
      return;
    }

    const auto *insert = (AstInsert *)root;

    std::vector<int> ast_idxs;
    GetTableHeaderWithAstMap(ast_idxs, insert->m_table, dbms, insert->m_collist.get(),
                             insert->m_vallist.get(), resp);
    auto header = resp.mutable_header();
    header->set_query_type(QueryType::QUERY_TYPE_INSERT);
    writer->Write(resp);

    // Insert only one column
    if (resp.has_err_status()) {
      return;
    }
    // Get column count from previous response
    size_t table_cols = resp.header().columns_size();
    auto **void_arr = (const void **)calloc(table_cols, sizeof(void *));
    // Create array for next responses
    auto *resp_col_arr = new DatabaseBodyColumn *[table_cols];

    // Create new response
    resp = DatabaseResponse();
    auto *resp_body = resp.mutable_body();

    for (size_t i = 0; i < table_cols; ++i) {
      resp_col_arr[i] = resp_body->add_columns();
    }

    // Copy row to void_arr and insert to body response
    size_t ast_idx = 0;
    for (const auto &ast_val : insert->m_vallist->m_lst) {
      size_t tar_idx = ast_idxs[ast_idx];
      void_arr[tar_idx] = getAstValuePtr(ast_val.get());
      resp_col_arr[tar_idx]->set_column_value(ast_val->strval);
      ++ast_idx;
    }

    auto *dto_row_list = dto_row_list_construct();
    dto_row_list_append(dto_row_list, void_arr);
    row_list_insert(dbms, insert->m_table.c_str(), dto_row_list);
    dto_row_list_destruct(&dto_row_list);
    // close resources and write row in response
    delete[] resp_col_arr;
    free(void_arr);
    writer->Write(resp);
  }

  // CREATE
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

  // DROP
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
