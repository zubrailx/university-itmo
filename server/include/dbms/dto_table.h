#pragma once

#include <stdbool.h>
#include <dbms/util/column_types.h>

typedef struct dto_table_column_limits {
  bool is_null;
  bool is_unique;
} dto_table_column_limits;

struct dto_table *dto_table_construct(const char *name);
void dto_table_destruct(struct dto_table **table_ptr);
void dto_table_add_column(struct dto_table *table, const char *name,
                          const enum table_column_type type,
                          const struct dto_table_column_limits lims);
int dto_table_column_cnt(const struct dto_table *table);
