#include "table.h"

void column_limits_to_page(struct dto_table_column_limits *from,
                           struct dpt_col_limits *to) {
  to->is_null = from->is_null;
}

void column_limits_to_dto(struct dpt_col_limits *from, struct dto_table_column_limits *to) {
  to->is_null = from->is_null;
}
