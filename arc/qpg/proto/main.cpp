#include <arc/qpg/proto.hpp>
#include <dbpb/ast.pb.h>
#include <stdexcept>

void serAstSubclass(const Ast &from, dbpb::AstSubclass &to);
void serStatementSubclass(const AstStatement &from, dbpb::AstStatementSubclass &to);

void serAstValue(const AstValue &from, dbpb::AstValue &to) {
  switch (from.m_dtype) {
  case DataType::BOOL:
    to.set_bval(from.getValue<bool>());
  case DataType::STR:
    to.set_sval(from.getValue<std::string>());
  case DataType::DOUBLE:
    to.set_fval(from.getValue<double>());
  case DataType::INT32:
    to.set_ival(from.getValue<int32_t>());
  default:
    throw std::invalid_argument("DataType is unexpected");
  }
}

template<typename T>
void serAstList(const AstList<T> &from, dbpb::AstList &to) {
  for (const auto &ast : from.m_lst) {
    auto db_elem = to.add_elements();
    serAstSubclass(from, *db_elem);
  }
}

void serAstColumn(const AstColumn &from, dbpb::AstColumn &to) {
  to.set_has_table(from.m_htn);
  to.set_table(from.m_table);
  to.set_column(from.m_column);
}

void serAstColumnList(const AstColumnList &from, dbpb::AstColumnList &to) {
  to.set_do_all(from.do_all);
  for (const auto &node : from.m_lst) {
    auto db_elem = to.add_columns();
    serAstColumn(*node.get(), *db_elem);
  }
}

void serAstTable(const AstTable &from, dbpb::AstTable &to) {
  to.set_table(from.m_name);
  to.set_has_alias(from.m_has_alias);
  to.set_alias(from.m_alias);
}

void serAstJoin(const AstJoin &from, dbpb::AstJoin &to) {
  to.set_join_type(static_cast<int32_t>(from.m_jtype));
  serAstSubclass(*from.m_lsv.get(), *to.mutable_lsv());
  serAstSubclass(*from.m_rsv.get(), *to.mutable_rsv());
}

void serStatementConst(const AstStatementConst &from, dbpb::AstStatementConst &to) {
  serAstValue(*from.m_res.get(), *to.mutable_value());
}

void serStatementColumn(const AstStatementColumn &from, dbpb::AstStatementColumn &to) {
  serAstColumn(*from.m_col.get(), *to.mutable_column());
}

void serStatementUnary(const AstStatementUnary &from, dbpb::AstStatementUnary &to) {
  to.set_op_type(static_cast<int32_t>(from.m_op));
  serStatementSubclass(*from.m_operand.get(), *to.mutable_operand());
}

void serStatementBinary(const AstStatementBinary &from, dbpb::AstStatementBinary &to) {
  to.set_op_type(static_cast<int32_t>(from.m_op));
  serStatementSubclass(*from.m_left.get(), *to.mutable_left());
  serStatementSubclass(*from.m_right.get(), *to.mutable_right());
}

void serStatementSubclass(const AstStatement &from, dbpb::AstStatementSubclass &to) {
  switch (from.m_stype) {
  case StatementType::CONST:
    serStatementConst(static_cast<const AstStatementConst &>(from),
                      *to.mutable_const_());
  case StatementType::COLUMN:
    serStatementColumn(static_cast<const AstStatementColumn &>(from),
                       *to.mutable_column());
  case StatementType::UNARY:
    serStatementUnary(static_cast<const AstStatementUnary &>(from),
                      *to.mutable_unary());
  case StatementType::BINARY:
    serStatementBinary(static_cast<const AstStatementBinary &>(from),
                       *to.mutable_binary());
  default:
    throw std::invalid_argument("StatementType is unexpected");
  }
}

// Serialize
void serSelect(const AstSelect &from, dbpb::AstSelect &to) {
  //   serAstColumnList(*from.m_collist.get(), *to.mutable_columns());
}

void serUpdate(const AstUpdate &from, dbpb::AstUpdate &to) {}

void serDelete(const AstDelete &from, dbpb::AstDelete &to) {}

void serInsert(const AstInsert &from, dbpb::AstInsert &to) {}

void serCreate(const AstCreate &from, dbpb::AstCreate &to) {}

void serDrop(const AstDrop &from, dbpb::AstDrop &to) {}

void serAstSubclass(const Ast &from, dbpb::AstSubclass &to) {
  switch (from.getType()) {
  case AstType::VARIABLE:
    serAstValue(static_cast<const AstValue &>(from), *to.mutable_value());
  case AstType::SELECT:
    serSelect(static_cast<const AstSelect &>(from), *to.mutable_select());
  case AstType::UPDATE:
    serUpdate(static_cast<const AstUpdate &>(from), *to.mutable_update());
  case AstType::DELETE:
    serDelete(static_cast<const AstDelete &>(from), *to.mutable_delete_());
  case AstType::INSERT:
    serInsert(static_cast<const AstInsert &>(from), *to.mutable_insert());
  case AstType::DROP:
    serDrop(static_cast<const AstDrop &>(from), *to.mutable_drop());
  case AstType::CREATE:
    serCreate(static_cast<const AstCreate &>(from), *to.mutable_create());
  default:
    throw std::invalid_argument("AstType is unexpected");
  }
}

void arc::qpg::proto::serialize(const Ast &from, dbpb::AstSubclass &to) {
  serAstSubclass(from, to);
}

// Deserialize
Ast *deserSelect(const dbpb::AstSubclass &from) {
  // auto root = new AstSelect();
}
Ast *deserUpdate(const dbpb::AstSubclass &from) {}
Ast *deserDelete(const dbpb::AstSubclass &from) {}
Ast *deserCreate(const dbpb::AstSubclass &from) {}
Ast *deserInsert(const dbpb::AstSubclass &from) {}
Ast *deserDrop(const dbpb::AstSubclass &from) {}

Ast *arc::qpg::proto::deserialize(const dbpb::AstSubclass &from) {
  switch (from.subclass_case()) {
  case dbpb::AstSubclass::kSelect:
    return deserSelect(from);
  case dbpb::AstSubclass::kUpdate:
    return deserUpdate(from);
  case dbpb::AstSubclass::kDelete:
    return deserDelete(from);
  case dbpb::AstSubclass::kCreate:
    return deserCreate(from);
  case dbpb::AstSubclass::kInsert:
    return deserInsert(from);
  case dbpb::AstSubclass::kDrop:
    return deserDrop(from);
  default:
    throw std::invalid_argument("Invalid argument in deserialize");
  }
  // switch (from.
}
