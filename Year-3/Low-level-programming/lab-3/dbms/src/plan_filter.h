#pragma once

#include <dbms/plan_filter.h>
#include <dbms/util/column_types.h>
#include <stddef.h>

// fast_const {{{
struct fast_const {
  struct fast base;

  struct dbms *dbms;
};
// }}}

// fast_column {{{
struct fast_column {
  struct fast base;

  struct dbms *dbms;// to manipulate with data that is not inlined

  char *table_name;
  char *column_name;
  // where column is stored
  size_t tbl_idx;
  size_t resc_off;
  size_t resc_size;// column size of result
};
// }}}

// fast_unop {{{
struct fast_unop {
  struct fast base;

  struct fast *parent;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(struct fast_unop *self, void *arg);// result is written res variable
};
// }}}

// fast_binop {{{
struct fast_binop {
  struct fast base;

  struct fast *left;
  struct fast *right;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(struct fast_binop *self, void *arg1, void *arg2);
};
// }}}
