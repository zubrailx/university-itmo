#include "qpg/ast.hpp"
#include <arc/qpg/proto.hpp>
#include <dbpb/ast.pb.h>
#include <stdexcept>

void serAstSubclass(const Ast &from, dbpb::AstSubclass &to);
void serAstStatementSubclass(const AstStatement &from, dbpb::AstStatementSubclass &to);
Ast *deserAstSubclass(const dbpb::AstSubclass &from);
AstStatement *deserAstStatementSubclass(const dbpb::AstStatementSubclass &from);

void serAstValue(const AstValue &from, dbpb::AstValue &to) {
  switch (from.m_dtype) {
  case DataType::BOOL:
    to.set_bval(from.getValue<bool>());
    break;
  case DataType::STR:
    to.set_sval(from.getValue<std::string>());
    break;
  case DataType::DOUBLE:
    to.set_fval(from.getValue<double>());
    break;
  case DataType::INT32:
    to.set_ival(from.getValue<int32_t>());
    break;
  default:
    throw std::invalid_argument("DataType is unexpected");
  }
}

template<typename T>
void serAstList(const AstList<T> &from, dbpb::AstList &to) {
  for (const auto &ast : from.m_lst) {
    auto db_elem = to.add_elements();
    serAstSubclass(*ast.get(), *db_elem);
  }
  to.set_ast_type(static_cast<int32_t>(from.getType()));
}

void serAstColumn(const AstColumn &from, dbpb::AstColumn &to) {
  if (from.m_htn) {
    to.set_table(from.m_table);
  }
  to.set_column(from.m_column);
}

void serAstColumnList(const AstColumnList &from, dbpb::AstColumnList &to) {
  to.set_do_all(from.do_all);
  for (const auto &node : from.m_lst) {
    auto db_elem = to.add_columns();
    serAstColumn(*node, *db_elem);
  }
}

void serAstTable(const AstTable &from, dbpb::AstTable &to) {
  to.set_table(from.m_name);
  if (from.m_has_alias) {
    to.set_alias(from.m_alias);
  }
}

void serAstJoin(const AstJoin &from, dbpb::AstJoin &to) {
  to.set_join_type(static_cast<int32_t>(from.m_jtype));
  serAstSubclass(*from.m_lsv, *to.mutable_lsv());
  serAstSubclass(*from.m_rsv, *to.mutable_rsv());
}

void serAstStatementConst(const AstStatementConst &from, dbpb::AstStatementConst &to) {
  serAstValue(*from.m_res, *to.mutable_value());
}

void serAstStatementColumn(const AstStatementColumn &from,
                           dbpb::AstStatementColumn &to) {
  serAstColumn(*from.m_col, *to.mutable_column());
}

void serAstStatementUnary(const AstStatementUnary &from, dbpb::AstStatementUnary &to) {
  to.set_op_type(static_cast<int32_t>(from.m_op));
  serAstStatementSubclass(*from.m_operand, *to.mutable_operand());
}

void serAstStatementBinary(const AstStatementBinary &from,
                           dbpb::AstStatementBinary &to) {
  to.set_op_type(static_cast<int32_t>(from.m_op));
  serAstStatementSubclass(*from.m_left, *to.mutable_left());
  serAstStatementSubclass(*from.m_right, *to.mutable_right());
}

void serAstStatementSubclass(const AstStatement &from, dbpb::AstStatementSubclass &to) {
  switch (from.m_stype) {
  case StatementType::CONST:
    serAstStatementConst(dynamic_cast<const AstStatementConst &>(from),
                         *to.mutable_const_());
    break;
  case StatementType::COLUMN:
    serAstStatementColumn(dynamic_cast<const AstStatementColumn &>(from),
                          *to.mutable_column());
    break;
  case StatementType::UNARY:
    serAstStatementUnary(dynamic_cast<const AstStatementUnary &>(from),
                         *to.mutable_unary());
    break;
  case StatementType::BINARY:
    serAstStatementBinary(dynamic_cast<const AstStatementBinary &>(from),
                          *to.mutable_binary());
    break;
  default:
    throw std::invalid_argument("StatementType is unexpected");
  }
}

void serAstSelect(const AstSelect &from, dbpb::AstSelect &to) {
  serAstColumnList(*from.m_collist, *to.mutable_list());
  serAstSubclass(*from.m_table_ref, *to.mutable_table_ref());
  if (from.m_has_cond) {
    serAstStatementSubclass(*from.m_cond, *to.mutable_condition());
  }
}

void serAstSubquery(const AstSubquery &from, dbpb::AstSubquery &to) {
  serAstSelect(*from.m_query, *to.mutable_query());
  if (from.m_has_alias) {
    to.set_alias(from.m_alias);
  }
}

void serAstColumnValue(const AstColumnValue &from, dbpb::AstColumnValue &to) {
  serAstColumn(*from.m_column, *to.mutable_column());
  serAstValue(*from.m_value, *to.mutable_value());
}

void serAstUpdate(const AstUpdate &from, dbpb::AstUpdate &to) {
  to.set_table(from.m_table);
  serAstList<AstColumnValue>(*from.m_collist, *to.mutable_collist());
  if (from.m_has_cond) {
    serAstStatementSubclass(*from.m_cond, *to.mutable_condition());
  }
}

void serAstDelete(const AstDelete &from, dbpb::AstDelete &to) {
  to.set_table(from.m_table);
  if (from.m_has_cond) {
    serAstStatementSubclass(*from.m_cond, *to.mutable_condition());
  }
}

void serAstInsert(const AstInsert &from, dbpb::AstInsert &to) {
  to.set_table(from.m_table);
  serAstColumnList(*from.m_collist, *to.mutable_columns());
  serAstList(*from.m_vallist, *to.mutable_values());
}

void serAstDrop(const AstDrop &from, dbpb::AstDrop &to) { to.set_table(from.m_table); }

void serAstColumnType(const AstColumnType &from, dbpb::AstColumnType &to) {
  serAstColumn(*from.m_column, *to.mutable_column());
  to.set_dtype(static_cast<int32_t>(from.m_type));
}

void serAstCreate(const AstCreate &from, dbpb::AstCreate &to) {
  to.set_table(from.m_table);
  serAstList<AstColumnType>(*from.m_collist, *to.mutable_columntypes());
}

void serAstSubclass(const Ast &from, dbpb::AstSubclass &to) {
  switch (from.getType()) {

  case AstType::VARIABLE:
    serAstValue(dynamic_cast<const AstValue &>(from), *to.mutable_value());
    break;

  case AstType::COLUMN:
    serAstColumn(dynamic_cast<const AstColumn &>(from), *to.mutable_column());
    break;

  case AstType::COLUMN_LIST:
    serAstColumnList(dynamic_cast<const AstColumnList &>(from),
                     *to.mutable_column_list());
    break;

  case AstType::TABLE:
    serAstTable(dynamic_cast<const AstTable &>(from), *to.mutable_table());
    break;

  case AstType::JOIN:
    serAstJoin(dynamic_cast<const AstJoin &>(from), *to.mutable_join());
    break;

  case AstType::STATEMENT:
    serAstStatementSubclass(dynamic_cast<const AstStatement &>(from),
                            *to.mutable_statement());
    break;

  case AstType::SELECT:
    serAstSelect(dynamic_cast<const AstSelect &>(from), *to.mutable_select());
    break;

  case AstType::SUBQUERY:
    serAstSubquery(dynamic_cast<const AstSubquery &>(from), *to.mutable_subquery());
    break;

  case AstType::COLUMN_VALUE:
    serAstColumnValue(dynamic_cast<const AstColumnValue &>(from),
                      *to.mutable_column_value());
    break;

  case AstType::UPDATE:
    serAstUpdate(dynamic_cast<const AstUpdate &>(from), *to.mutable_update());
    break;

  case AstType::DELETE:
    serAstDelete(dynamic_cast<const AstDelete &>(from), *to.mutable_delete_());
    break;

  case AstType::INSERT:
    serAstInsert(dynamic_cast<const AstInsert &>(from), *to.mutable_insert());
    break;

  case AstType::DROP:
    serAstDrop(dynamic_cast<const AstDrop &>(from), *to.mutable_drop());
    break;

  case AstType::COLUMN_TYPE:
    serAstColumnType(dynamic_cast<const AstColumnType &>(from),
                     *to.mutable_column_type());
    break;

  case AstType::CREATE:
    serAstCreate(dynamic_cast<const AstCreate &>(from), *to.mutable_create());
    break;

  default:
    throw std::invalid_argument("AstType is unexpected");
  }
}

void arc::qpg::proto::serialize(const Ast &from, dbpb::AstSubclass &to) {
  serAstSubclass(from, to);
}

// Deserialize
AstValue *deserAstValue(const dbpb::AstValue &from) {
  switch (from.value_case()) {
  case dbpb::AstValue::kIval:
    return new AstValue(from.ival(), DataType::INT32);
  case dbpb::AstValue::kFval:
    return new AstValue(from.fval(), DataType::DOUBLE);
  case dbpb::AstValue::kSval:
    return new AstValue(from.sval(), DataType::STR);
  case dbpb::AstValue::kBval:
    return new AstValue(from.bval(), DataType::BOOL);
  default:
    throw std::invalid_argument("AstValue value is not set");
  }
}

template<typename T>
AstList<T> *deserAstList(const dbpb::AstList &list) {
  std::list<std::unique_ptr<T>> lst;
  for (const auto &node : list.elements()) {
    auto elem = deserAstSubclass(node);
    lst.push_back(std::unique_ptr<T>((T *)elem));
  }
  return new AstList<T>(std::move(lst), static_cast<AstType>(list.ast_type()));
}

AstColumn *deserAstColumn(const dbpb::AstColumn &from) {
  if (from.has_table()) {
    return new AstColumn(from.table().c_str(), from.column().c_str());
  } else {
    return new AstColumn(from.column().c_str());
  }
}

AstColumnList *deserAstColumnList(const dbpb::AstColumnList &from) {
  if (from.do_all()) {
    return new AstColumnList();
  } else {
    std::list<std::unique_ptr<AstColumn>> collist;
    for (const auto &node : from.columns()) {
      collist.emplace_back(deserAstColumn(node));
    }
    return new AstColumnList(std::move(collist));
  }
}

AstTable *deserAstTable(const dbpb::AstTable &from) {
  if (from.has_alias()) {
    return new AstTable(std::string(from.table()), from.alias().c_str());
  } else {
    return new AstTable(std::string(from.table()));
  }
}

AstJoin *deserAstJoin(const dbpb::AstJoin &from) {
  auto lsv = deserAstSubclass(from.lsv());
  auto rsv = deserAstSubclass(from.rsv());
  return new AstJoin(lsv, static_cast<JoinType>(from.join_type()), rsv);
}

AstStatementConst *deserAstStatementConst(const dbpb::AstStatementConst &from) {
  auto value = deserAstValue(from.value());
  return new AstStatementConst(value);
}

AstStatementColumn *deserAstStatementColumn(const dbpb::AstStatementColumn &from) {
  auto column = deserAstColumn(from.column());
  return new AstStatementColumn(column);
}

AstStatementUnary *deserAstStatementUnary(const dbpb::AstStatementUnary &from) {
  auto operand = deserAstStatementSubclass(from.operand());
  return new AstStatementUnary(operand, static_cast<OperationType>(from.op_type()));
}

AstStatementBinary *deserAstStatementBinary(const dbpb::AstStatementBinary &from) {
  auto left = deserAstStatementSubclass(from.left());
  auto right = deserAstStatementSubclass(from.right());
  return new AstStatementBinary(left, right,
                                static_cast<OperationType>(from.op_type()));
}

AstStatement *deserAstStatementSubclass(const dbpb::AstStatementSubclass &from) {
  switch (from.subclass_case()) {

  case dbpb::AstStatementSubclass::kConst:
    return deserAstStatementConst(from.const_());

  case dbpb::AstStatementSubclass::kColumn:
    return deserAstStatementColumn(from.column());

  case dbpb::AstStatementSubclass::kUnary:
    return deserAstStatementUnary(from.unary());

  case dbpb::AstStatementSubclass::kBinary:
    return deserAstStatementBinary(from.binary());

  default:
    throw std::invalid_argument("AstStatement type is unexpected");
  }
}

AstSelect *deserAstSelect(const dbpb::AstSelect &from) {
  auto collist = deserAstColumnList(from.list());
  auto table_ref = deserAstSubclass(from.table_ref());
  if (from.has_condition()) {
    auto condition = deserAstStatementSubclass(from.condition());
    return new AstSelect(collist, table_ref, condition);
  } else {
    return new AstSelect(collist, table_ref);
  }
}

AstSubquery *deserAstSubquery(const dbpb::AstSubquery &from) {
  auto select = deserAstSelect(from.query());
  if (from.has_alias()) {
    return new AstSubquery(select, from.alias().c_str());
  } else {
    return new AstSubquery(select);
  }
}

AstColumnValue *deserAstColumnValue(const dbpb::AstColumnValue &from) {
  auto value = deserAstValue(from.value());
  auto column = from.column().column();
  return new AstColumnValue(column.c_str(), value);
}

Ast *deserUpdate(const dbpb::AstUpdate &from) {
  const auto& table = from.table();
  auto list = deserAstList<AstColumnValue>(from.collist());
  if (from.has_condition()) {
    auto cond = deserAstStatementSubclass(from.condition());
    return new AstUpdate(table.c_str(), list, cond);
  } else {
    return new AstUpdate(table.c_str(), list);
  }
}
Ast *deserDelete(const dbpb::AstDelete &from) {
  const auto& table = from.table();
  if (from.has_condition()) {
    auto cond = deserAstStatementSubclass(from.condition());
    return new AstDelete(table.c_str(), cond);
  } else {
    return new AstDelete(table.c_str());
  }
}

Ast *deserInsert(const dbpb::AstInsert &from) {
  const auto& table = from.table();
  auto collist = deserAstColumnList(from.columns());
  auto vallist = deserAstList<AstValue>(from.values());
  return new AstInsert(table.c_str(), collist, vallist);
}

Ast *deserDrop(const dbpb::AstDrop &from) {
  const auto& table = from.table();
  return new AstDrop(table.c_str());
}

AstColumnType *deserAstColumnType(const dbpb::AstColumnType &from) {
  auto column = deserAstColumn(from.column());
  return new AstColumnType(column, static_cast<DataType>(from.dtype()));
}

Ast *deserCreate(const dbpb::AstCreate &from) {
  const auto& table = from.table();
  auto collist = deserAstList<AstColumnType>(from.columntypes());
  return new AstCreate(table.c_str(), collist);
}

Ast *deserAstSubclass(const dbpb::AstSubclass &from) {
  switch (from.subclass_case()) {

  case dbpb::AstSubclass::kValue:
    return static_cast<Ast *>(deserAstValue(from.value()));

  case dbpb::AstSubclass::kList:
    return static_cast<Ast *>(deserAstList<Ast>(from.list()));

  case dbpb::AstSubclass::kColumn:
    return static_cast<Ast *>(deserAstColumn(from.column()));

  case dbpb::AstSubclass::kColumnList:
    return static_cast<Ast *>(deserAstColumnList(from.column_list()));

  case dbpb::AstSubclass::kTable:
    return static_cast<Ast *>(deserAstTable(from.table()));

  case dbpb::AstSubclass::kJoin:
    return static_cast<Ast *>(deserAstJoin(from.join()));

  case dbpb::AstSubclass::kStatement:
    return static_cast<Ast *>(deserAstStatementSubclass(from.statement()));

  case dbpb::AstSubclass::kSelect:
    return static_cast<Ast *>(deserAstSelect(from.select()));

  case dbpb::AstSubclass::kSubquery:
    return static_cast<Ast *>(deserAstSubquery(from.subquery()));

  case dbpb::AstSubclass::kColumnValue:
    return static_cast<Ast *>(deserAstColumnValue(from.column_value()));

  case dbpb::AstSubclass::kUpdate:
    return static_cast<Ast *>(deserUpdate(from.update()));

  case dbpb::AstSubclass::kDelete:
    return static_cast<Ast *>(deserDelete(from.delete_()));

  case dbpb::AstSubclass::kInsert:
    return static_cast<Ast *>(deserInsert(from.insert()));

  case dbpb::AstSubclass::kDrop:
    return static_cast<Ast *>(deserDrop(from.drop()));

  case dbpb::AstSubclass::kColumnType:
    return static_cast<Ast *>(deserAstColumnType(from.column_type()));

  case dbpb::AstSubclass::kCreate:
    return static_cast<Ast *>(deserCreate(from.create()));

  default:
    throw std::invalid_argument("AstType is unexpected");
  }
}

Ast *arc::qpg::proto::deserialize(const dbpb::AstSubclass &from) {
  return deserAstSubclass(from);
}
