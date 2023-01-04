#pragma once

#include <dto_table.h>
#include <stdbool.h>

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
