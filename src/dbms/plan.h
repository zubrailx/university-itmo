#pragma once

#include <stdint.h>

#include "io/page/p_database.h"
#include "io/page/p_table.h"

enum plan_type {
  PLAN_TYPE_SOURCE,
  PLAN_TYPE_TERMINAL,
  PLAN_TYPE_UNOP,
  PLAN_TYPE_BINOP,

  PLAN_TYPE_DELETED,
  PLAN_TYPE_ERROR,
  PLAN_TYPE_SELECT
};

#define OVERRIDE
#define INHERIT

struct plan_table_info {
  const char *table_name;
  struct dp_tuple *dpt;
  size_t tpt_size;
  struct tpt_col_info *col_info;
};

struct plan {
  enum plan_type type;
  // info about what get returns
  size_t arr_size;
  struct plan_table_info *pti_arr;
  struct tp_tuple **tuple_arr;

  const struct plan_table_info *(*get_info)(void *self);
  // get row with structure of plan_row_info arr
  struct tp_tuple **(*get)(void *self);
  bool (*next)(void *self);
  // Check if this element is last
  bool (*end)(void *self);
  void (*destruct)(void *self);
  // returns true if page if can row be located in db (if not virtual) else false
  bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

struct plan_source {
  struct plan base;

  struct dbms *dbms;
  // iterator
  struct tp_iter *iter;

  INHERIT const struct plan_table_info *(*get_info)(void *self);
  INHERIT struct tp_tuple **(*get)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
  OVERRIDE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

struct plan_projection {};

struct plan_predicate {};

// returns data like it is received from plan_source (smth like virtual table)
// but don't write data
struct plan_select {
  struct plan base;

  struct plan *parent;

  // methods
  void (*get_header)(void *self, struct dpt_column **out, size_t *arr_size_out);

  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
};
struct plan_update {};
struct plan_delete {};

struct plan_source plan_source_construct(const void *table_name, struct dbms *dbms);

#undef OVERRIDE
#undef INHERIT
