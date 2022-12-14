#pragma once

#include "io/page/column_types.h"

#define EXT_UNOP(name) extern struct fast_unop_func name
#define EXT_BINOP(name) extern struct fast_binop_func name

struct fast_unop;
struct fast_binop;

// fast_unop {{{
struct fast_unop_func {
  void (*func)(struct fast_unop *self, void *arg);
  enum table_column_type ret_type;
};

EXT_UNOP(BOOL_NOT);
// }}}

// fast_binop {{{
struct fast_binop_func {
  void (*func)(struct fast_binop *self, void *arg1, void *arg2);
  enum table_column_type ret_type;
};

EXT_BINOP(DOUBLE_LARGER);
EXT_BINOP(DOUBLE_EQUALS);

EXT_BINOP(INT32_EQUALS);
EXT_BINOP(INT32_LARGER);

EXT_BINOP(BOOL_OR);
EXT_BINOP(BOOL_AND);

EXT_BINOP(STRING_EQUALS);
// }}}

// FUNCS
