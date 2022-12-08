#include "table.h"

enum table_column_type column_type_to_page(const enum dto_table_column_type from) {
  switch ((uint8_t)from) {
  case DTO_COLUMN_BOOL:
    return COLUMN_TYPE_BOOL;
  case DTO_COLUMN_FLOAT:
    return COLUMN_TYPE_DOUBLE;
  case DTO_COLUMN_INT:
    return COLUMN_TYPE_INT32;
  case DTO_COLUMN_STRING:
    return COLUMN_TYPE_STRING;
  }
  return COLUMN_TYPE_UNKNOWN;
}

void column_limits_to_page(struct dto_table_column_limits *from,
                           struct dpt_col_limits *to) {
  to->is_null = from->is_null;
}
