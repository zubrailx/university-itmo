#include "converters.hpp"

table_column_type toTableColumnType(DataType dtype) {
  switch(dtype) {
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
  switch(type) {
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
