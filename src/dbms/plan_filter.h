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
  // result of calc
  struct tpt_column_base *tptc;
  enum table_column_type tptc_type;

  void (*compile)(void *self, size_t pti_size, struct plan_table_info *info_arr);
  void (*destruct)(void *self);

  void *(*calc)(void *self);// void * - returned result
  void (*pass)(void *self, const struct tp_tuple **tuple_arr);
};

struct fast_const {
  struct fast base;
};
struct fast_const *fast_const_construct(enum dto_table_column_type col_type,
                                        void *value);

struct fast_column {
  struct fast base;

  struct dbms *dbms;// to manipulate with data that is not inlined

  char *table_name;
  char *column_name;
  // where column is stored
  size_t tbl_idx;
  size_t tptc_off;
  size_t tptc_size;// column size
};
struct fast_column *fast_column_construct(const char *table_name,
                                          const char *column_name, struct dbms *dbms);

struct fast_unop {
  struct fast base;

  struct fast *parent;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(void *self, void *arg);// result is written res variable
};
struct fast_unop *fast_unop_construct(void *parent, void (*func)(void *, void *),
                                      enum table_column_type type, struct dbms *dbms);

struct fast_binop {
  struct fast base;

  struct fast *left;
  struct fast *right;

  struct dbms *dbms;// to manipulate with data that is not inlined

  void (*func)(void *self, void *arg1, void *arg2);
};
struct fast_binop *fast_binop_construct(void *p_left, void *p_right,
                                        void (*func)(void *, void *, void *),
                                        enum table_column_type type, struct dbms *dbms);
