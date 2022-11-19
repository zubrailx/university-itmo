#pragma once

#include "../../page/database/typle.h"

my_defstruct(dto_table_column);
struct dto_table_column {
  enum table_column_types type;
  struct dpt_col_limits lims;
  char *name;
  struct dto_table_column *next;
};

my_defstruct(dto_table);
struct dto_table {
  struct {
    char *name;
  } header;
  struct dto_table_column *first;
  struct dto_table_column *last;
};

struct dto_table *dto_typle_construct(char *name);
void dto_typle_destruct(struct dto_table **typle_ptr);
void dto_typle_add_column(struct dto_table *typle, char *name,
                          enum table_column_types type, struct dpt_col_limits lims);
