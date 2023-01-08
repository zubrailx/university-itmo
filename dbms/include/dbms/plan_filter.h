#pragma once

#include <dbms/util/column_types.h>
#include <stddef.h>

struct dbms;
struct fast_binop_func;
struct fast_unop_func;
struct plan_table_info;
struct tp_tuple;

enum fast_type { FAST_UNOP, FAST_BINOP, FAST_CONST, FAST_COLUMN, FAST_FREED };

// fast {{{
struct fast {
  enum fast_type type;
  // result of calc
  void *res;
  enum table_column_type res_type;

  void (*compile)(void *self, size_t pti_size, const struct plan_table_info *info_arr,
                  const char **err_msg);
  void (*destruct)(void *self);

  void *(*calc)(void *self);// void * - returned result
  void (*pass)(void *self, const struct tp_tuple **tuple_arr);
};
// }}}

struct fast_const *fast_const_construct(enum table_column_type col_type,
                                        const void *value, struct dbms *dbms);

struct fast_column *fast_column_construct(const char *table_name,
                                          const char *column_name, struct dbms *dbms,
                                          const char **err_msg);

struct fast_unop *fast_unop_construct(void *parent, const struct fast_unop_func *fuf,
                                      struct dbms *dbms);

struct fast_binop *fast_binop_construct(void *p_left, void *p_right,
                                        const struct fast_binop_func *fbf,
                                        struct dbms *dbms);
