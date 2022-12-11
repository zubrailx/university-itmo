#pragma once

#include <stdint.h>

#include "io/page/p_table.h"

enum plan_type {
  PLAN_TYPE_SOURCE,
  PLAN_TYPE_TERMINAL,
  PLAN_TYPE_UNOP,
  PLAN_TYPE_BINOP,

  PLAN_TYPE_DELETED,
  PLAN_TYPE_ERROR
};

struct plan_row_info {
  const char *table_name;
  struct tpt_col_info *info;
};

struct plan {
  enum plan_type type;
  // info about what get returns
  size_t arr_size;
  struct plan_row_info *info_arr;
  struct tp_tuple **tuple_arr;

  // Methods
  const struct plan_row_info *(*get_info)(void *self);

  // virtual
  // get row with structure of plan_row_info arr
  struct tp_tuple **(*get)(void *self);
  bool (*next)(void *self);
  // Check if this element is last
  bool (*end)(void *self);
  void (*destruct)(void *self);
};

struct plan_source {
  struct plan base;

  struct dbms *dbms;
  // dpt points inside dp
  struct database_page *dp;
  struct fileoff_t dp_loc;
  struct dp_tuple *dpt;
  // iterator
  struct tp_iter *iter;

  // override
  const struct plan_row_info *(*get_info)(void *self);
  struct tp_tuple **(*get)(void *self);
  bool (*next)(void *self);
  bool (*end)(void *self);
  void (*destruct)(void *self);
};

struct plan_projection {};

struct plan_predicate {};

struct plan_select {};
struct plan_update {};
struct plan_delete {};

struct plan_source plan_source_construct(const void *table_name, struct dbms *dbms);
