#pragma once

#include <stddef.h>

struct tp_tuple;

enum filter_ast_type { FAST_UNOP, FAST_BINOP, FAST_BINOP_CONST };

struct filter_ast {
  enum filter_ast_type type;

  size_t arr_size;
  struct plan_table_info *info_arr;

  void (*compile)(void *self, struct plan_table_info *info_arr);
  void (*destruct)(void *self);

  bool (*pass)(void *self, const struct tp_tuple **tuple_arr);
};
