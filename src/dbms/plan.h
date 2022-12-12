#pragma once

#include <stdint.h>

#include "io/page/p_database.h"
#include "io/page/p_table.h"

struct dbms;

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
#define PRIVATE
#define VIRTUAL

struct plan_table_info {
  const char *table_name;
  struct dp_tuple *dpt;// only valid for current plan node
  size_t tpt_size;
  struct tpt_col_info *col_info;
};

struct plan {
  enum plan_type type;
  // info about what get returns
  size_t arr_size;
  struct plan_table_info *pti_arr;
  struct tp_tuple **tuple_arr;

  VIRTUAL const struct plan_table_info *(*get_info)(void *self);
  // get row with structure of plan_row_info arr
  VIRTUAL struct tp_tuple **(*get)(void *self);
  VIRTUAL bool (*next)(void *self);
  // Check if this element is last
  VIRTUAL bool (*end)(void *self);
  VIRTUAL void (*destruct)(void *self_ptr);

  // returns true if page if can row be located in db (if not virtual) else false
  VIRTUAL PRIVATE bool (*locate)(void *self, fileoff_t *fileoff, pageoff_t *pageoff);
  VIRTUAL PRIVATE bool (*get_page)(void *self, table_page **page_out);
  VIRTUAL PRIVATE bool (*get_dbms)(void *self, struct dbms **dmbs_out);
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
};

struct plan_update {
  struct PLAN_PARENT;
  // tuple_arr is used to store new rows (maybe for postprocessing)
  struct dbms *dbms;

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
};

struct plan_delete {
  struct PLAN_PARENT;

  struct dbms *dbms;
  // tuple_arr contains NULLs
  INHERIT const struct plan_table_info *(*get_info)(void *self);
  INHERIT struct tp_tuple **(*get)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE bool (*end)(void *self);
  OVERRIDE void (*destruct)(void *self);
};

struct plan_predicate {};

struct plan_source *plan_source_construct(const void *table_name, struct dbms *dbms,
                                          bool do_write);
struct plan_select *plan_select_construct_move(void *parent_void,
                                               const char *table_name);

#undef PLAN_PARENT
#undef VIRTUAL
#undef PRIVATE
#undef OVERRIDE
#undef INHERIT
