#pragma once

#include "dto/dto_table.h"
#include "io/page/column_types.h"
#include "io/page/p_table.h"
#include <stddef.h>

struct tp_tuple;
struct plan_table_info;

enum fast_type { FAST_UNOP, FAST_BINOP, FAST_CONST, FAST_COLUMN, FAST_FREED };

struct fast {

  enum fast_type type;

  void (*compile)(void *self, struct plan_table_info *info_arr);
  void (*destruct)(void *self);

  void *(*calc)(void *self);// void * - returned result
  void (*pass)(void *self, const struct tp_tuple **tuple_arr);
};

struct fast_const {
  struct fast base;

  enum table_column_type tpt_type;
  struct tpt_column_base *tpt_col;
};
struct fast_const *fast_const_construct(enum dto_table_column_type col_type,
                                        void *value);

struct fast_column {
  struct fast base;

  // where required array is stored
  size_t pti_idx;
  size_t dpt_col;

  enum table_column_type tpt_type;
  struct tpt_column_base *tpt_col;
};

struct fast_unop {
  struct fast base;

  enum table_column_type res_type;
  struct tpt_column_base *res;// result of operation

  void (*func)(void *arg);
};

struct fast_binop {
  struct fast base;

  enum table_column_type res_type;
  struct tpt_column_base *res;// result of operation

  void (*func)(void *arg1, void *arg2);
};
