#pragma once

#include "sso.h"

// FILE
my_defstruct(dpt_header);
struct dpt_header {
  fileoff_t fileoff;// first page of table
  size_t cols;
  struct page_sso sso;
};

enum table_column_types {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
};

my_defstruct(dpt_col_limits);
struct dpt_col_limits {
  bool is_null;
  bool is_unique;
};

my_defstruct(dpt_column);
struct dpt_column {
  /* TableColumnType */
  int8_t type;
  struct dpt_col_limits limits;
  struct page_sso sso;
};

my_defstruct(dp_typle);
struct dp_typle {
  struct dpt_header header;
  struct dpt_column columns[];
} __attribute__((packed));
