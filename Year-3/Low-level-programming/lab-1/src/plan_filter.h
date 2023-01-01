#pragma once

#include <stddef.h>
#include <util/column_types.h>

struct tp_tuple;
struct plan_table_info;

enum fast_type { FAST_UNOP, FAST_BINOP, FAST_CONST, FAST_COLUMN, FAST_FREED };

// fast {{{
struct fast {
  enum fast_type type;
  // result of calc
  void *res;
  enum table_column_type res_type;

  void (*compile)(void *self, size_t pti_size, struct plan_table_info *info_arr);
  void (*destruct)(void *self);

  void *(*calc)(void *self);// void * - returned result
  void (*pass)(void *self, const struct tp_tuple **tuple_arr);
};
// }}}

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
