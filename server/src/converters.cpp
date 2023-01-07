#include "converters.hpp"
#include <cstdint>

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

std::string toString(const AstValue *ref) {

  switch(ref->m_dtype) {
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

// convert to types for table to read
void *getAstValuePtr(AstValue *ref) {
  switch(ref->m_dtype) {
    case DataType::STR:
      return (void *)ref->getValueRef<std::string>()->c_str();
    case DataType::BOOL:
      return ref->getValueRef<bool>();
    case DataType::DOUBLE:
      return ref->getValueRef<double>();
    case DataType::INT32:
      return ref->getValueRef<int32_t>();
    default:
      return nullptr;
  }
}
