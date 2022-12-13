#include "plan.h"
#include "io/page/p_table.h"
#include "iter.h"
#include "op_schema.h"
#include "op_table.h"
#include "page.h"
#include "table.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// something like virtual override in c++
#define VIRT_OVERRIDE(self, base, method) self.base.method = self.method
#define VIRT_INHERIT(self, base, method) self.method = self.base.method

// plan {{{
static const struct plan_table_info *plan_get_info(void *self) {
  return ((struct plan *)self)->pti_arr;
}
static struct tp_tuple **plan_get(void *self) {
  return ((struct plan *)self)->tuple_arr;
}

static bool plan_next(void *self) { return false; }

static bool plan_end(void *self_void) {
  struct plan *self = self_void;
  for (size_t i = 0; i < self->arr_size; ++i) {
    if (self->tuple_arr[i] != NULL) {
      return false;
    }
  }
  return true;
}

static void plan_destruct(struct plan *self) {
  // free row info
  if (self->pti_arr) {
    for (int i = 0; i < self->arr_size; ++i) {
      struct plan_table_info *ie = self->pti_arr + i;
      if (ie->table_name) {
        free((void *)ie->table_name);
        ie->table_name = NULL;
      }
      if (ie->col_info) {
        free(ie->col_info);
        ie->col_info = NULL;
      }
      if (ie->dpt) {
        free(ie->dpt);
        ie->dpt = NULL;
      }
    }
    free(self->pti_arr);
    self->pti_arr = NULL;
  }
  // free tuple_arr
  if (self->tuple_arr) {
    for (int i = 0; i < self->arr_size; ++i) {
      if (self->tuple_arr[i]) {
        free(self->tuple_arr[i]);
        self->tuple_arr[i] = NULL;
      }
    }
    free(self->tuple_arr);
    self->tuple_arr = NULL;
  }
  self->type = PLAN_TYPE_DELETED;
  free(self);
}

static void plan_destruct_void(void *self_ptr) { plan_destruct(self_ptr); }

// PRIVATE
static bool plan_get_iter(void *self, struct tp_iter **iter_out) { return false; }

static bool plan_get_dbms(void *self, struct dbms **dbms_out) { return false; }

static void plan_start(void *self, bool do_write) {}

// constructor
struct plan plan_construct(enum plan_type type, size_t arr_size) {
  return (struct plan){
      .type = type,
      .arr_size = arr_size,
      .pti_arr = malloc(sizeof(struct plan_table_info) * arr_size),
      .tuple_arr = calloc(sizeof(struct tp_tuple *) * arr_size, 1),
      // methods
      .get_info = plan_get_info,
      .get = plan_get,

      .next = plan_next,
      .end = plan_end,
      .destruct = plan_destruct_void,

      .get_iter = plan_get_iter,
      .get_dbms = plan_get_dbms,
      .start = plan_start,
  };
}

// for tuple_arr only allocated array of pointers
static void plan_set_pti_shallow(struct plan *plan,
                                 const struct plan_table_info arr[]) {
  size_t info_size = sizeof(struct plan_table_info) * plan->arr_size;
  memcpy(plan->pti_arr, arr, info_size);
}

void plan_set_pti_deep(struct plan *plan, const struct plan_table_info arr[]) {

  plan_set_pti_shallow(plan, arr);

  for (size_t i = 0; i < plan->arr_size; ++i) {
    struct plan_table_info *pti = plan->pti_arr + i;

    size_t cols = arr[i].dpt->header.cols;
    size_t dpt_size = dp_tuple_size(cols);

    pti->dpt = malloc(dpt_size);
    memcpy(pti->dpt, arr[i].dpt, dpt_size);

    pti->table_name = strdup(arr[i].table_name);

    size_t col_info_size = sizeof(struct tpt_col_info) * cols;
    pti->col_info = malloc(col_info_size);
    memcpy(pti->col_info, arr[i].col_info, col_info_size);
  }
}
// }}}

// plan_source {{{
static bool plan_source_next(void *self_void) {
  struct plan_source *self = self_void;
  bool res = tp_iter_next(self->iter);
  self->base.tuple_arr[0] = tp_iter_get(self->iter);
  return res;
}

static void plan_source_destruct(void *self_void) {
  struct plan_source *self = self_void;
  if (self->iter) {
    tp_iter_destruct(&self->iter);
  }
  self->base.tuple_arr[0] = NULL;
  // call base destructor
  plan_destruct(&self->base);
}

static bool plan_source_get_iter(void *self_void, struct tp_iter **iter_out) {
  struct plan_source *self = self_void;
  *iter_out = self->iter;
  return true;
}

static bool plan_source_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_source *self = self_void;
  *dbms_out = self->dbms;
  return true;
}

static void plan_source_start(void *self_void, bool do_write) {
  struct plan_source *self = self_void;
  // restart iterator
  tp_iter_destruct(&self->iter);
  self->iter = tp_iter_construct(self->dbms, self->base.pti_arr[0].dpt, do_write);
  self->base.tuple_arr[0] = tp_iter_get(self->iter);
}

static void plan_source_start_public(void *self_void) {
  plan_source_start(self_void, false);
}

// @do_write - do open on write
struct plan_source *plan_source_construct(const void *table_name, struct dbms *dbms) {
  struct plan_source *self = my_malloc(struct plan_source);
  // specific for source
  self->dbms = dbms;
  pageoff_t dpt_off;
  fileoff_t dpt_loc;
  if (!dbms_find_table(table_name, dbms, &dpt_loc, &dpt_off)) {
    printf("plan_source_construct: Table not found.\n");
    goto err;
  }

  struct dp_tuple *plan_dpt;

  {// construct
    database_page *dp;
    const struct dp_tuple *dpt = dbms_select_tuple(dpt_loc, dpt_off, dbms, &dp);

    size_t dpt_size = dp_tuple_size(dpt->header.cols);
    plan_dpt = malloc(dpt_size);
    memcpy(plan_dpt, dpt, dpt_size);

    dp_destruct(&dp);
  }
  {// set base structure field
    struct plan_table_info table_info = {.table_name = strdup(table_name),
                                         .dpt = plan_dpt,
                                         .tpt_size = tp_get_tuple_size(plan_dpt),
                                         .col_info =
                                             tp_construct_col_info_arr(plan_dpt)};

    self->base = plan_construct(PLAN_TYPE_SOURCE, 1);
    self->iter = NULL;
    plan_set_pti_shallow(&self->base, &table_info);
  }
  {
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, end);

    (*self).next = plan_source_next;
    VIRT_OVERRIDE((*self), base, next);

    (*self).destruct = plan_source_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.get_iter = plan_source_get_iter;
    self->base.get_dbms = plan_source_get_dbms;
    self->base.start = plan_source_start;
  }
  return self;
err:
  printf("plan_source_construct: Failed.\n");
  plan_source_destruct(self);
  return self;
}
// }}}

// plan_parent {{{
static bool plan_parent_next(void *self_void) {
  struct plan_parent *self = self_void;
  bool res = self->parent->next(self->parent);

  if (res) {
    struct tp_tuple **p_tuple = self->parent->get(self->parent);
    for (size_t i = 0; i < self->base.arr_size; ++i) {
      self->base.tuple_arr[i] = p_tuple[i];
    }
  } else {
    for (size_t i = 0; i < self->base.arr_size; ++i) {
      self->base.tuple_arr[i] = NULL;
    }
  }
  return res;
}

static void plan_parent_destruct(void *self_void) {
  struct plan_parent *self = self_void;
  // free parent node
  if (self->parent) {
    self->parent->destruct(self->parent);
  }
  // call base destructor
  plan_destruct(&self->base);
}

static bool plan_parent_get_iter(void *self_void, struct tp_iter **iter_out) {
  struct plan_parent *self = self_void;
  return self->parent->get_iter(self->parent, iter_out);
}

static bool plan_parent_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_parent *self = self_void;
  return self->parent->get_dbms(self->parent, dbms_out);
}

static void plan_parent_start(void *self_void, bool do_write) {
  struct plan_parent *self = self_void;
  self->parent->start(self->parent, do_write);
}

static void plan_parent_start_public(void *self_void) {
  struct plan_parent *self = self_void;
  self->parent->start(self->parent, false);
}
// }}}

// plan_select{{{
static void tpt_flatten(struct tp_tuple *dest, struct tp_tuple **src,
                        const struct plan *parent) {
  dest->header.is_present = false;

  size_t cur_off = offsetof(tp_tuple, columns);
  for (size_t i = 0; i < parent->arr_size; ++i) {
    size_t icol_size = parent->pti_arr[i].tpt_size - offsetof(tp_tuple, columns);
    if (src[i] != NULL) {
      memcpy((void *)dest + cur_off, src[i]->columns, icol_size);
    }
    cur_off += icol_size;
  }
}

static bool plan_select_next(void *self_void) {
  struct plan_select *self = self_void;
  const struct plan *parent = self->parent;

  bool res = self->parent->next(self->parent);
  if (res) {
    struct tp_tuple **tuple_src = parent->get(self->parent);
    struct tp_tuple *tuple_dest = self->base.tuple_arr[0];

    tpt_flatten(tuple_dest, tuple_src, parent);
  } else {
    free(self->base.tuple_arr[0]);
    self->base.tuple_arr[0] = NULL;
  }
  return res;
}

static dp_tuple *dpt_flatten_malloc(size_t size, const struct plan_table_info *src) {
  struct dp_tuple *dest;

  size_t total_cols = 0;
  for (size_t i = 0; i < size; ++i) {
    // calculate total amount of columns to create dp_tuple
    total_cols += src[i].dpt->header.cols;
  }
  {
    dest = malloc(dp_tuple_size(total_cols));
    // Header
    // zero everything until columns
    memset(dest, 0, offsetof(struct dp_tuple, columns));
    dest->header.is_present = false;
    dest->header.cols = total_cols;
    // Columns
    size_t cur_col = 0;
    for (size_t i = 0; i < size; ++i) {
      const size_t icols = src[i].dpt->header.cols;
      const size_t icol_size = dp_tuple_size(icols) - offsetof(dp_tuple, columns);

      memcpy(dest->columns + cur_col, src[i].dpt->columns, icol_size);
      cur_col += icols;
    }
  }
  return dest;
}

static void plan_select_start(void *self_void, bool do_write) {
  struct plan_select *self = self_void;
  struct plan *parent = self->parent;

  if (self->base.tuple_arr[0]) {
    free(self->base.tuple_arr[0]);
  }
  self->base.tuple_arr[0] = malloc(self->base.pti_arr[0].tpt_size);

  parent->start(parent, do_write);
  tpt_flatten(self->base.tuple_arr[0], parent->get(parent), parent);
}

static void plan_select_start_public(void *self) { plan_select_start(self, false); }

// @parent_void - moved
struct plan_select *plan_select_construct_move(void *parent_void,
                                               const char *table_name) {
  struct plan_select *self = my_malloc(struct plan_select);

  struct plan *parent = parent_void;
  self->parent = parent;
  parent_void = NULL;

  {// base
    const struct plan_table_info *parent_info = parent->get_info(parent);

    struct dp_tuple *dpt = dpt_flatten_malloc(parent->arr_size, parent_info);

    struct plan_table_info table_info = {.table_name = strdup(table_name),
                                         .dpt = dpt,
                                         .tpt_size = tp_get_tuple_size(dpt),
                                         .col_info = tp_construct_col_info_arr(dpt)};

    self->base = plan_construct(PLAN_TYPE_SELECT, 1);
    plan_set_pti_shallow(&self->base, &table_info);
  }
  {
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, end);

    self->next = plan_select_next;
    VIRT_OVERRIDE((*self), base, next);

    self->destruct = plan_parent_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.get_iter = plan_parent_get_iter;
    self->base.get_dbms = plan_parent_get_dbms;
    self->base.start = plan_select_start;

    self->start = plan_select_start_public;
  }
  return self;
}
// }}}

// plan_update {{{
static void plan_update_update(void *self_void) {
  struct plan_update *self = self_void;

  // tp_iter_get == self->parent->tuple_arr[0]
  tp_tuple *tpt_old = tp_iter_get(self->iter);// get should return true
  tp_tuple *tpt_new = self->base.tuple_arr[0];
  const struct plan_table_info pti = self->base.pti_arr[0];
  const struct dp_tuple *dpt = pti.dpt;

  // update new tuple columns
  memcpy(tpt_new, tpt_old, pti.tpt_size);
  for (size_t i = 0; i < self->col_size; ++i) {
    size_t idx = self->col_idxs[i];
    const void *val = self->col_vals[i];

    void *tpt_new_col = (void *)tpt_new + pti.col_info[idx].start;
    const uint8_t col_type = dpt->columns[idx].type;
    tpt_memcpy_specific(tpt_new_col, val, col_type, self->dbms);
  }
  tp_iter_update_columns(self->iter, tpt_new, pti.col_info, self->col_size,
                         self->col_idxs);
}

static bool plan_update_next(void *self_void) {
  struct plan_update *self = self_void;

  struct plan *parent = self->parent;
  bool res = parent->next(parent);
  if (res) {
    plan_update_update(self);
  } else {
    free(self->base.tuple_arr[0]);
    self->base.tuple_arr[0] = NULL;
  }
  return res;
}

static void plan_update_destruct(void *self_void) {
  struct plan_update *self = self_void;
  if (self->col_idxs) {
    free(self->col_idxs);
    self->col_idxs = NULL;
  }
  if (self->col_vals) {
    free(self->col_vals);
    self->col_vals = NULL;
  }
  plan_parent_destruct(self);
}

static bool plan_update_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_update *self = self_void;
  *dbms_out = self->dbms;
  return true;
}

static bool plan_update_get_iter(void *self_void, struct tp_iter **iter_out) {
  struct plan_update *self = self_void;
  *iter_out = self->iter;
  return true;
}

static void plan_update_start(void *self_void, bool do_write) {
  struct plan_update *self = self_void;
  self->parent->start(self->parent, do_write);

  // update iterator
  self->parent->get_iter(self->parent, &self->iter);

  if (self->base.tuple_arr[0]) {
    free(self->base.tuple_arr[0]);
  }
  // Set tuple_arr (new are stored)
  if (self->parent->end(self->parent)) {
    self->base.tuple_arr = NULL;
  } else {
    self->base.tuple_arr[0] = malloc(self->base.pti_arr[0].tpt_size);
    plan_update_update(self);
  }
}

static void plan_update_start_public(void *self_void) {
  struct plan_update *self = self_void;
  self->base.start(self, true);
}

struct plan_update *plan_update_construct_move(void *parent_void, size_t cv_size,
                                               struct column_value *arr) {
  struct plan_update *self = my_malloc(struct plan_update);
  *self = (struct plan_update){};

  // Set parent
  struct plan *parent = parent_void;
  self->parent = parent;
  parent_void = NULL;

  {
    // construct
    self->base = plan_construct(PLAN_TYPE_UPDATE, 1);
    // Checks
    const struct plan_table_info *parent_info = parent->get_info(parent);
    if (parent->arr_size != 1 || !parent_info[0].dpt->header.is_present) {
      printf("plan_update_construct: Parent iterates over virtual table.\n");
      goto err;
    }
    // Set dbms and check
    bool res = parent->get_dbms(parent, &self->dbms);
    if (!res) {
      printf("plan_update_construct: Doesn't have access to file.\n");
      goto err;
    }
    // Set iter
    self->iter = NULL;
    // Set pti
    plan_set_pti_deep(&self->base, parent_info);
    // Set cols to update
    self->col_size = cv_size;
    self->col_idxs = malloc(sizeof(*self->col_idxs) * cv_size);
    self->col_vals = malloc(sizeof(*self->col_vals) * cv_size);

    for (size_t i = 0; i < cv_size; ++i) {
      int idx = dbms_find_column_idx(self->base.pti_arr[0].dpt, arr[i].column_name,
                                     self->dbms);
      if (idx < 0) {
        printf("plan_update_construct: Columns name '%s' not found.\n",
               arr[i].column_name);
        goto err;
      } else {
        self->col_idxs[i] = (size_t)idx;
        self->col_vals[i] = arr[i].column_value;
      }
    }
  }
  {
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, end);

    self->next = plan_update_next;
    VIRT_OVERRIDE((*self), base, next);

    self->destruct = plan_update_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.get_iter = plan_update_get_iter;
    self->base.get_dbms = plan_update_get_dbms;
    self->base.start = plan_update_start;

    self->start = plan_update_start_public;
  }
  return self;

err:
  printf("plan_update_construct: Failed.\n");
  plan_update_destruct(self);
  return self;
}
// }}}

// plan_delete {{{
static void plan_delete_delete(void *self_void) {
  struct plan_delete *self = self_void;
  tp_iter_remove(self->iter, self->base.pti_arr[0].col_info);
}

static bool plan_delete_next(void *self_void) {
  struct plan_delete *self = self_void;
  struct plan *parent = self->parent;

  bool res = parent->next(parent);
  if (res) {
    plan_delete_delete(self);
  }
  return res;
}

static bool plan_delete_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_delete *self = self_void;
  *dbms_out = self->dbms;
  return true;
}

static bool plan_delete_get_iter(void *self_void, struct tp_iter **iter_out) {
  struct plan_delete *self = self_void;
  *iter_out = self->iter;
  return true;
}

static void plan_delete_start(void *self_void, bool do_write) {
  struct plan_delete *self = self_void;
  self->parent->start(self->parent, do_write);

  self->parent->get_iter(self->parent, &self->iter);

  if (!self->parent->end(self->parent)) {
    plan_delete_delete(self);
  }
}

static void plan_delete_start_public(void *self_void) {
  struct plan_delete *self = self_void;
  self->base.start(self, true);
}

struct plan_delete *plan_delete_construct_move(void *parent_void) {
  struct plan_delete *self = my_malloc(struct plan_delete);
  *self = (struct plan_delete){};
  // move
  struct plan *parent = parent_void;
  self->parent = parent;
  parent_void = NULL;

  {
    self->base = plan_construct(PLAN_TYPE_DELETE, 1);
    // Checks
    const struct plan_table_info *parent_info = parent->get_info(parent);
    if (parent->arr_size != 1 || !parent_info[0].dpt->header.is_present) {
      printf("plan_delete_construct: Parent iterates over virtual table.\n");
      goto err;
    }
    // Set dbms and check
    bool res = parent->get_dbms(parent, &self->dbms);
    if (!res) {
      printf("plan_delete_construct: Doesn't have access to file.\n");
      goto err;
    }
    // Set iter
    self->iter = NULL;
    // Set pti
    plan_set_pti_deep(&self->base, parent_info);
  }
  {
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, end);

    self->next = plan_delete_next;
    VIRT_OVERRIDE((*self), base, next);

    self->destruct = plan_parent_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.get_iter = plan_delete_get_iter;
    self->base.get_dbms = plan_delete_get_dbms;
    self->base.start = plan_delete_start;

    self->start = plan_delete_start_public;
  }
  return self;

err:
  printf("plan_delete_construct: Failed.\n");
  plan_update_destruct(self);
  return self;
}
// }}}

#undef VIRT_OVERRIDE
