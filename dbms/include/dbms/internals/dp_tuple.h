#pragma once

#include <dbms/internals/sso.h>

typedef struct dpt_header {
  bool is_present;
  size_t cols;
  struct page_sso sso;

  fileoff_t tp_first; /* first and last page of the table */
  fileoff_t tp_last;

  fileoff_t td_last; /* pointer to last elements in gappy */
} dpt_header;

typedef struct dpt_col_limits {
  bool is_null;
} dpt_col_limits;

typedef struct dpt_column {
  int8_t type; /* table_column_type */
  struct dpt_col_limits limits;
  struct page_sso sso;
} dpt_column;

typedef struct dp_tuple {
  struct dpt_header header;
  struct dpt_column columns[];
} dp_tuple;

