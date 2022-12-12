#pragma once

#include <stdint.h>

#include "io/page/p_database.h"
#include "io/page/p_table.h"

enum plan_type {
  PLAN_TYPE_SOURCE,

  PLAN_TYPE_DELETED,
  PLAN_TYPE_ERROR,

  PLAN_TYPE_SELECT,
  PLAN_TYPE_UPDATE,
  PLAN_TYPE_DELETE

  // PLAN_TYPE_TERMINAL,
  // PLAN_TYPE_UNOP,
  // PLAN_TYPE_BINOP,
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
  void (*destruct)(void *self_ptr);

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
  OVERRIDE void (*destruct)(void *self_ptr);
  OVERRIDE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

#define PLAN_PARENT                                                                    \
  {                                                                                    \
    struct plan base;                                                                  \
    struct plan *parent;                                                               \
  }

struct plan_parent {
  struct PLAN_PARENT;
};

struct plan_projection {};

// returns data like it is received from plan_source (smth like virtual table)
// but don't write data

struct plan_select {
  struct PLAN_PARENT;

  // methods
  INHERIT const struct plan_table_info *(*get_info)(void *self);
  INHERIT struct tp_tuple **(*get)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
  OVERRIDE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

struct plan_update {
  struct PLAN_PARENT;
  // tuple_arr is used to store new rows (maybe for postprocessing)

  INHERIT const struct plan_table_info *(*get_info)(void *self);
  // returns NEW values
  INHERIT struct tp_tuple **(*get)(void *self);
  
  // Get OLD value
  const tp_tuple **(*old)(void *self);
  // Perform update (NEW -> OLD)
  void (*set)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
  OVERRIDE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

struct plan_delete {
  struct PLAN_PARENT;
  // tuple_arr contains NULLs

  INHERIT const struct plan_table_info *(*get_info)(void *self);
  INHERIT struct tp_tuple **(*get)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
  OVERRIDE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
};

struct plan_predicate {};

struct plan_source *plan_source_construct(const void *table_name, struct dbms *dbms,
                                          bool do_write);
struct plan_select *plan_select_construct_move(void *parent_void,
                                               const char *table_name);

#undef PLAN_PARENT
#undef OVERRIDE
#undef INHERIT
