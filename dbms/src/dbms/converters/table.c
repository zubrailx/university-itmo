#include "table.h"
#include "../sso.h"
#include <dbms/internals/dto_tuple.h>
#include <stdlib.h>

void column_limits_to_page(struct dto_table_column_limits *from,
                           struct dpt_col_limits *to) {
  to->is_null = from->is_null;
}

void column_limits_to_dto(struct dpt_col_limits *from,
                          struct dto_table_column_limits *to) {
  to->is_null = from->is_null;
}

void dp_tuple_to_dto(struct dp_tuple *tuple, struct dto_table **dto_out,
                     struct dbms *dbms) {
  const char *table_name = dbms_sso_construct_select(&tuple->header.sso, dbms);
  struct dto_table *table = dto_table_construct(table_name);
  free((void *)table_name);

  for (size_t i = 0; i < tuple->header.cols; ++i) {
    dpt_column column = tuple->columns[i];
    const char *col_name = dbms_sso_construct_select(&column.sso, dbms);
    dto_table_column_limits lims;
    column_limits_to_dto(&column.limits, &lims);
    dto_table_add_column(table, col_name, column.type, lims);
  }
}
