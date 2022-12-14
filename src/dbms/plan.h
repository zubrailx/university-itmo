#pragma once

#include <stdint.h>

#include "io/page/p_database.h"
#include "io/page/p_table.h"

struct dbms;
struct tp_iter;

enum plan_type {
  PLAN_TYPE_SOURCE,

  PLAN_TYPE_DELETED,
  PLAN_TYPE_ERROR,

  PLAN_TYPE_SELECT,
  PLAN_TYPE_UPDATE,
  PLAN_TYPE_DELETE,

  PLAN_TYPE_CROSS_JOIN,

  PLAN_TYPE_FILTER,
};

#define OVERRIDE
#define INHERIT
#define PRIVATE
#define VIRTUAL

// plan {{{
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

  VIRTUAL const struct plan_table_info *(*get_info)(void *self, size_t *size);
  // get row with structure of plan_row_info arr
  VIRTUAL struct tp_tuple **(*get)(void *self);
  VIRTUAL bool (*next)(void *self);
  // Check if this element is last
  VIRTUAL bool (*end)(void *self);
  VIRTUAL void (*destruct)(void *self);

  // returns true if page if can row be located in db (if not virtual) else false
  VIRTUAL PRIVATE bool (*get_iter)(void *self, struct tp_iter **iter_out);
  VIRTUAL PRIVATE bool (*get_dbms)(void *self, struct dbms **dbms_out);
  VIRTUAL PRIVATE void (*start)(void *self, bool do_write);
};
// }}}

// plan_source {{{
struct plan_source {
  struct plan base;

  struct dbms *dbms;
  // iterator
  struct tp_iter *iter;

  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self_ptr);
};

struct plan_source *plan_source_construct(const void *table_name, struct dbms *dbms);
// }}}

#define PLAN_PARENT                                                                    \
  {                                                                                    \
    struct plan base;                                                                  \
    struct plan *parent;                                                               \
  }

struct plan_parent {
  struct PLAN_PARENT;
};

// plan_select{{{
// returns data like it is received from plan_source (smth like virtual table)
// but don't write data
struct plan_select {
  struct PLAN_PARENT;
  // methods
  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self);

  void (*start)(void *self);// only present in terminal plan_nodes
};

struct plan_select *plan_select_construct_move(void *parent_void,
                                               const char *table_name);
// }}}
// plan_update {{{
struct plan_update {
  // in tuple_arr stores new values (pointers to parent values because they are updated)
  struct PLAN_PARENT;
  // tuple_arr is used to store new rows (maybe for postprocessing)
  struct dbms *dbms;
  struct tp_iter *iter;

  // cols to update
  size_t col_size;
  size_t *col_idxs;     // array
  const void **col_vals;// array

  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  // get parent rows -> update -> parent rows updated
  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self);

  void (*start)(void *self);// only present in terminal plan_nodes
};

struct column_value {
  const char *column_name;
  const void *column_value;
};

struct plan_update *plan_update_construct_move(void *parent_void, size_t size,
                                               struct column_value *arr);
// }}}

// plan_delete {{{
struct plan_delete {
  struct PLAN_PARENT;

  struct dbms *dbms;
  struct tp_iter *iter;
  // tuple_arr contains NULLs
  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self);

  void (*start)(void *self);// only present in terminal plan_nodes
};

struct plan_delete *plan_delete_construct_move(void *parent_void);
// }}}

// plan_cross_join {{{
struct plan_cross_join {
  struct plan base;

  struct plan *p_left;
  struct plan *p_right;

  bool do_write;

  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self);
};

struct plan_cross_join *plan_cross_join_construct_move(void *parent_left,
                                                       void *parent_right);
// }}}

// plan_filter {{{
struct plan_filter {
  struct PLAN_PARENT;

  struct fast *filt;

  INHERIT const struct plan_table_info *(*get_info)(void *self, size_t *size);
  INHERIT struct tp_tuple **(*get)(void *self);
  INHERIT bool (*end)(void *self);

  OVERRIDE bool (*next)(void *self);
  OVERRIDE void (*destruct)(void *self);
};

struct plan_filter *plan_filter_construct_move(void *parent, void *filt);
// }}}

#undef PLAN_PARENT
#undef VIRTUAL
#undef PRIVATE
#undef OVERRIDE
#undef INHERIT
