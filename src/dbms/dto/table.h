#pragma once

#include <stdbool.h>

enum dto_table_column_type {
  DTO_COLUMN_INT = 0,
  DTO_COLUMN_FLOAT,
  DTO_COLUMN_STRING,
  DTO_COLUMN_BOOL
};

typedef struct dto_table_column_limits {
  bool is_null;
  bool is_unique;
} dto_table_column_limits;

typedef struct dto_table_column {
  char *name;
  enum dto_table_column_type type;
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
                          const enum dto_table_column_type type,
                          const struct dto_table_column_limits lims);
