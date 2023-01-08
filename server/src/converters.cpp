#include "converters.hpp"
#include <fmt/core.h>
#include <ios>

extern "C" {
#include <dbms/internals/dp_tuple.h>
#include <dbms/plan_funcs.h>
#include <dbms/tp_tuple.h>
#include <dbms/util/column_types.h>
}

#include <cstdint>
#include <qpg/ast.hpp>

table_column_type toTableColumnType(DataType dtype) {
  switch (dtype) {
  case DataType::BOOL:
    return COLUMN_TYPE_BOOL;
  case DataType::INT32:
    return COLUMN_TYPE_INT32;
  case DataType::STR:
    return COLUMN_TYPE_STRING;
  case DataType::DOUBLE:
    return COLUMN_TYPE_DOUBLE;
  default:
    return COLUMN_TYPE_UNKNOWN;
  }
}

std::string toString(table_column_type type) {
  switch (type) {
  case COLUMN_TYPE_BOOL:
    return "bool";
  case COLUMN_TYPE_INT32:
    return "int32";
  case COLUMN_TYPE_STRING:
    return "string";
  case COLUMN_TYPE_DOUBLE:
    return "double";
  default:
    return "unknown";
  }
}

std::string toString(const AstValue *ref) {

  switch (ref->m_dtype) {
  case DataType::STR:
    return ref->getValue<std::string>();
  case DataType::BOOL:
    return std::to_string(ref->getValue<bool>());
  case DataType::DOUBLE:
    return std::to_string(ref->getValue<double>());
  case DataType::INT32:
    return std::to_string(ref->getValue<int32_t>());
  default:
    return std::string();
  }
}

std::string toString(const struct tpt_column_base *tpt_base,
                     const struct dpt_column *dpt_column, struct dbms *dbms) {
  return std::string(tp_column_to_str(tpt_base, dpt_column, dbms));
}

// convert to types for table to read
const void *getAstValuePtr(AstValue *ref) {
  switch (ref->m_dtype) {
  case DataType::STR:
    return ref->getValueRef<std::string>().c_str();
  case DataType::BOOL:
    return &ref->getValueRef<bool>();
  case DataType::DOUBLE:
    return &ref->getValueRef<double>();
  case DataType::INT32:
    return &ref->getValueRef<int32_t>();
  default:
    return nullptr;
  }
}

static const struct fast_unop_func *unsupportedFunc(OperationType op_type,
                                                    table_column_type dtype) {
  std::cout << fmt::format("Unsupported op type: {}, ({})\n", (int)op_type, (int)dtype);
  return &UNARY_UNDEF;
}

static const struct fast_binop_func *unsupportedFunc(OperationType op_type,
                                                     table_column_type left,
                                                     table_column_type right) {
  std::cout << fmt::format("Unsupported op type: {}, ({}, {}) \n", (int)op_type,
                           (int)left, (int)right);
  return &BINARY_UNDEF;
}

const struct fast_unop_func *toDbmsFunc(OperationType optype, table_column_type dtype) {
  switch (optype) {
  case OperationType::NOT: {
    switch (dtype) {
    case COLUMN_TYPE_BOOL:
      return &BOOL_NOT;
    default:
      return unsupportedFunc(optype, dtype);
    }
  default:
    return unsupportedFunc(optype, dtype);
  }
  }
}

const struct fast_binop_func *toDbmsFunc(OperationType optype, table_column_type left,
                                         table_column_type right) {
  if (left != right) {
    return unsupportedFunc(optype, left, right);
  }

  switch (optype) {

  case OperationType::AND: {
    switch (left) {
    case COLUMN_TYPE_BOOL:
      return &BOOL_AND;
    default:
      return unsupportedFunc(optype, left, right);
    }
  }

  case OperationType::OR: {
    switch (left) {
    case COLUMN_TYPE_BOOL:
      return &BOOL_OR;
    default:
      return unsupportedFunc(optype, left, right);
    }
  }

  case OperationType::EQ: {
    switch (left) {
    case COLUMN_TYPE_DOUBLE:
      return &DOUBLE_EQUALS;
    case COLUMN_TYPE_INT32:
      return &INT32_EQUALS;
    case COLUMN_TYPE_BOOL:
      return &BOOL_EQUALS;
    case COLUMN_TYPE_STRING:
      return &STRING_EQUALS;
    default:
      return unsupportedFunc(optype, left, right);
    }
  }

  case OperationType::GR: {
    switch (left) {
    case COLUMN_TYPE_DOUBLE:
      return &DOUBLE_LARGER;
    case COLUMN_TYPE_INT32:
      return &INT32_LARGER;
    default:
      return unsupportedFunc(optype, left, right);
    }
  }

  case OperationType::IN: {
    switch (left) {
    case COLUMN_TYPE_STRING:
      return &STRING_IN;
    default:
      return unsupportedFunc(optype, left, right);
    }
  }

  default:
    return unsupportedFunc(optype, left, right);
  }
}
