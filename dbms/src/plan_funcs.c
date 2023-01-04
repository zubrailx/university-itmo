#include <dbms/plan_funcs.h>

#include "dbms/io/page/p_table.h"
#include "dbms/sso.h"
#include "plan_filter.h"
#include <stdlib.h>
#include <string.h>

// fast_unop {{{
static void bool_not(struct fast_unop *self, void *arg_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) * arg, *res;
  arg = arg_void;
  res = self->base.res;

  res->is_null = arg->is_null;
  res->entry = !arg->entry;
}
struct fast_unop_func BOOL_NOT = {.func = bool_not, .ret_type = COLUMN_TYPE_BOOL};

// }}}

// fast_binop {{{
static void double_larger(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_DOUBLE) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  res->is_null = arg1->is_null || arg2->is_null;
  res->entry = arg1->entry > arg2->entry;
}
struct fast_binop_func DOUBLE_LARGER = {.func = double_larger,
                                        .ret_type = COLUMN_TYPE_BOOL};

static void double_equals(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_DOUBLE) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  res->is_null = arg1->is_null || arg2->is_null;
  res->entry = arg1->entry == arg2->entry;
}
struct fast_binop_func DOUBLE_EQUALS = {.func = double_equals,
                                        .ret_type = COLUMN_TYPE_BOOL};

static void int32_larger(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_INT32) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  res->is_null = arg1->is_null || arg2->is_null;
  res->entry = arg1->entry > arg2->entry;
}
struct fast_binop_func INT32_LARGER = {.func = int32_larger,
                                       .ret_type = COLUMN_TYPE_BOOL};

static void int32_equals(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_INT32) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  res->is_null = arg1->is_null || arg2->is_null;
  res->entry = arg1->entry == arg2->entry;
}
struct fast_binop_func INT32_EQUALS = {.func = int32_equals,
                                       .ret_type = COLUMN_TYPE_BOOL};

static void bool_or(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  res->is_null = arg1->is_null && arg2->is_null;
  if (arg1->is_null && arg2->is_null) {
    res->is_null = true;
  } else {
    res->is_null = false;
    if (arg1->is_null) {
      res->entry = arg2->entry;
    } else if (arg2->is_null) {
      res->entry = arg1->entry;
    } else {
      res->entry = arg1->entry || arg2->entry;
    }
  }
}
struct fast_binop_func BOOL_OR = {.func = bool_or, .ret_type = COLUMN_TYPE_BOOL};

static void bool_and(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  if (!arg1->is_null && !arg2->is_null) {
    res->is_null = false;
    res->entry = arg1->entry && arg2->entry;
  } else {
    if ((arg1->is_null && !arg2->entry) || (!arg1->entry && arg2->is_null)) {
      res->entry = false;
    } else {
      res->is_null = true;
    }
  }
}
struct fast_binop_func BOOL_AND = {.func = bool_and, .ret_type = COLUMN_TYPE_BOOL};

static void string_equals(struct fast_binop *self, void *arg1_void, void *arg2_void) {
  struct TPT_COL_TYPE(COLUMN_TYPE_STRING) * arg1, *arg2;
  arg1 = arg1_void, arg2 = arg2_void;
  struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *res = self->base.res;

  if (arg1->is_null || arg2->is_null) {
    res->is_null = true;
    return;
  } else {
    res->is_null = false;
  }

  const char *str1 = dbms_sso_construct_select(&arg1->entry, self->dbms);
  const char *str2 = dbms_sso_construct_select(&arg2->entry, self->dbms);
  res->entry = !strcmp(str1, str2);
  free((void *)str1);
  free((void *)str2);
}
struct fast_binop_func STRING_EQUALS = {.func = string_equals,
                                        .ret_type = COLUMN_TYPE_BOOL};
// }}}
