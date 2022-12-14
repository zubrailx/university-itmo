#pragma once

#include "dto/dto_table.h"
#include "io/page/column_types.h"
#include "io/page/p_table.h"
#include <stddef.h>

struct tp_tuple;
struct plan_table_info;

struct fast_binop_func;
struct fast_unop_func;

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
};
struct fast_const *fast_const_construct(enum dto_table_column_type col_type,
                                        void *value);
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
struct fast_column *fast_column_construct(const char *table_name,
                                          const char *column_name, struct dbms *dbms);
// }}}

// fast_unop {{{
struct fast_unop {
  struct fast base;

  struct fast *parent;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(struct fast_unop *self, void *arg);// result is written res variable
};
struct fast_unop *fast_unop_construct(void *parent, struct fast_unop_func *fuf,
                                      struct dbms *dbms);
// }}}

// fast_binop {{{
struct fast_binop {
  struct fast base;

  struct fast *left;
  struct fast *right;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(struct fast_binop *self, void *arg1, void *arg2);
};
struct fast_binop *fast_binop_construct(void *p_left, void *p_right,
                                        struct fast_binop_func *fbf, struct dbms *dbms);
// }}}
