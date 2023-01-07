#pragma once

#include <dbms/util/column_types.h>
#include <stdbool.h>

typedef struct dto_table_column_limits {
  bool is_null;
  bool is_unique;
} dto_table_column_limits;

typedef struct dto_table_column {
  char *name;
  enum table_column_type type;
  struct dto_table_column_limits lims;
  struct dto_table_column *next;
} dto_table_column;

typedef struct dto_table {
  char *name;
  struct dto_table_column *first;
  struct dto_table_column *last;
} dto_table;

struct dto_table *dto_table_construct(const char *name);
void dto_table_destruct(struct dto_table **table_ptr);
void dto_table_add_column(struct dto_table *table, const char *name,
                          const enum table_column_type type,
                          const struct dto_table_column_limits lims);
int dto_table_column_cnt(const struct dto_table *table);
