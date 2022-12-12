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
static void plan_destruct(struct plan *self) {
  self->type = PLAN_TYPE_DELETED;
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
  free(self);
}

static const struct plan_table_info *plan_get_info(void *self) {
  return ((struct plan *)self)->pti_arr;
}
static struct tp_tuple **plan_get(void *self) {
  return ((struct plan *)self)->tuple_arr;
}

static bool plan_next(void *self) { return false; }

static bool plan_end(void *self) { return true; }

static void plan_destruct_void(void *self_ptr) { plan_destruct(self_ptr); }

static bool plan_locate(void *self, fileoff_t *fileoff, pageoff_t *pageoff) {
  return false;
}

static bool plan_get_page(void *self, table_page **page_out) { return false; }

static bool plan_get_dbms(void *self, struct dbms **dbms_out) { return false; }

struct plan plan_construct(enum plan_type type, size_t arr_size) {
  return (struct plan){.type = type,
                       .arr_size = arr_size,
                       .pti_arr = malloc(sizeof(struct plan_table_info) * arr_size),
                       .tuple_arr = malloc(sizeof(struct tp_tuple *) * arr_size),
                       // methods
                       .get_info = plan_get_info,
                       .get = plan_get,

                       .next = plan_next,
                       .end = plan_end,
                       .destruct = plan_destruct_void,

                       .locate = plan_locate,
                       .get_page = plan_get_page,
                       .get_dbms = plan_get_dbms};
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

    memcpy(pti->col_info, arr[i].col_info, sizeof(tpt_col_info) * cols);
  }
}
// }}}

// plan_source {{{
static void plan_source_destruct(void *self_void) {
  struct plan_source *self = self_void;
  if (self->iter) {
    tp_iter_destruct(&self->iter);
  }
  // call base destructor
  plan_destruct(&self->base);
}

static bool plan_source_next(void *self_void) {
  struct plan_source *self = self_void;
  bool res = tp_iter_next(self->iter);
  self->base.tuple_arr[0] = tp_iter_get(self->iter);
  return res;
}

static bool plan_source_end(void *self_void) {
  struct plan_source *self = self_void;
  return tp_iter_get(self->iter) == NULL;
}

static bool plan_source_locate(void *self_void, fileoff_t *loc, pageoff_t *off) {
  *loc = tp_iter_cur_page(((struct plan_source *)self_void)->iter);
  *off = tp_iter_cur_tuple(((struct plan_source *)self_void)->iter);
  return true;
}

static bool plan_source_get_page(void *self_void, table_page **page_out) {
  struct plan_source *self = self_void;
  *page_out = tp_iter_get_page(self->iter);
  return true;
}

static bool plan_source_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_source *self = self_void;
  *dbms_out = self->dbms;
  return true;
}

// @do_write - do open on write
struct plan_source *plan_source_construct(const void *table_name, struct dbms *dbms,
                                          bool do_write) {
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

  {
    database_page *dp;
    const struct dp_tuple *dpt = dbms_select_tuple(dpt_loc, dpt_off, dbms, &dp);

    size_t dpt_size = dp_tuple_size(dpt->header.cols);
    plan_dpt = malloc(dpt_size);
    memcpy(plan_dpt, dpt, dpt_size);

    dp_destruct(&dp);
  }

  self->iter = tp_iter_construct(dbms, plan_dpt, do_write);

  {// set base structure field
    struct plan_table_info table_info = {.table_name = strdup(table_name),
                                         .dpt = plan_dpt,
                                         .tpt_size = tp_get_tuple_size(plan_dpt),
                                         .col_info =
                                             tp_construct_col_info_arr(plan_dpt)};

    self->base = plan_construct(PLAN_TYPE_SOURCE, 1);
    plan_set_pti_shallow(&self->base, &table_info);
    self->base.tuple_arr[0] = tp_iter_get(self->iter);
  }

  {
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, get);

    (*self).next = plan_source_next;
    VIRT_OVERRIDE((*self), base, next);

    (*self).end = plan_source_end;
    VIRT_OVERRIDE((*self), base, end);

    (*self).destruct = plan_source_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.locate = plan_source_locate;
    self->base.get_page = plan_source_get_page;
    self->base.get_dbms = plan_source_get_dbms;
  }
  return self;
err:
  printf("plan_source_construct: Failed.\n");
  plan_source_destruct(self);
  return self;
}
// }}}

// plan_parent {{{
static void plan_parent_destruct(void *self_void) {
  struct plan_parent *self = self_void;
  // free parent node
  if (self->parent) {
    self->parent->destruct(self->parent);
  }
  // call base destructor
  plan_destruct(&self->base);
}

static bool plan_parent_next(void *self_void) {
  struct plan_parent *self = self_void;
  return self->parent->next(self->parent);
}

static bool plan_parent_end(void *self_void) {
  struct plan_parent *self = self_void;
  return self->parent->end(self->parent);
}

static bool plan_parent_locate(void *self_void, fileoff_t *fileoff,
                               pageoff_t *pageoff) {
  struct plan_parent *self = self_void;
  return self->parent->locate(self->parent, fileoff, pageoff);
}

static bool plan_parent_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_parent *self = self_void;
  return self->parent->get_dbms(self->parent, dbms_out);
}

static bool plan_parent_get_page(void *self_void, table_page **page_out) {
  struct plan_parent *self = self_void;
  return self->parent->get_page(self->parent, page_out);
}

static const tp_tuple **plan_parent_old(void *self_void) {
  struct plan_parent *self = self_void;
  return (const tp_tuple **)self->parent->get(self->parent);
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

  bool res = plan_parent_next(self);
  if (!res) {
    return false;
  }
  struct tp_tuple **tuple_src = parent->get(self->parent);
  struct tp_tuple *tuple_dest = self->base.tuple_arr[0];

  tpt_flatten(tuple_dest, tuple_src, parent);

  return true;
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
    // allocate tuple_arr[0] and set to initial value
    self->base.tuple_arr[0] = malloc(self->base.pti_arr[0].tpt_size);
    tpt_flatten(self->base.tuple_arr[0], parent->get(parent), parent);
  }
  {
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, get_info);

    self->next = plan_select_next;
    VIRT_OVERRIDE((*self), base, next);

    self->end = plan_parent_end;
    VIRT_OVERRIDE((*self), base, end);

    self->destruct = plan_parent_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->base.locate = plan_parent_locate;
    self->base.get_page = plan_parent_get_page;
    self->base.get_dbms = plan_parent_get_dbms;
  }
  return self;
}
// }}}

// plan_update {{{
static void plan_update_set(void *self_void) {
  struct plan_update *self = self_void;
  struct plan *parent = self->parent;

  table_page *tp;
  struct dbms *dbms;
  bool is_virt = !parent->get_page(parent, &tp);
  bool is_virt2 = !parent->get_dbms(parent, &dbms);
  assert(!(is_virt || is_virt2));

  for (size_t i = 0; i < self->base.arr_size; ++i) {
    const tp_tuple *tpt_new = self->base.tuple_arr[i];
    tp_tuple *tpt_old = parent->tuple_arr[i];
    const struct plan_table_info pti = self->base.pti_arr[i];

    dbms_update_row_pre(tpt_old, tpt_new, pti.dpt, pti.col_info, dbms);
    tp_update_row_ptr(tp, tpt_new, pti.tpt_size, tpt_old);
  }
}

static bool plan_update_get_dbms(void *self_void, struct dbms **dbms_out) {
  struct plan_update *self = self_void;
  *dbms_out = self->dbms;
  return true;
}

struct plan_update *plan_update_construct_move(void *parent_void,
                                               const char *table_name) {
  struct plan_update *self = my_malloc(struct plan_update);

  struct plan *parent = parent_void;
  self->parent = parent;
  parent_void = NULL;

  {
    const struct plan_table_info *parent_info = parent->get_info(parent);

    if (parent->arr_size != 1 || !parent_info[0].dpt->header.is_present) {
      printf("plan_update_construct: Parent iterates over virtual table.\n");
      goto err;
    }

    self->base = plan_construct(PLAN_TYPE_UPDATE, 1);

    bool res = parent->get_dbms(parent, &self->dbms);
    if (!res) {
      printf("plan_update_construct: Doesn't have access to file.\n");
      goto err;
    }

    plan_set_pti_deep(&self->base, parent_info);
    self->base.tuple_arr[0] = malloc(self->base.pti_arr[0].tpt_size);
  }
  {
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, get);

    self->next = plan_parent_next;
    VIRT_OVERRIDE((*self), base, next);

    self->end = plan_parent_end;
    VIRT_OVERRIDE((*self), base, end);

    self->destruct = plan_parent_destruct;
    VIRT_OVERRIDE((*self), base, destruct);

    self->set = plan_update_set;
    self->old = plan_parent_old;

    self->base.locate = plan_parent_locate;
    self->base.get_page = plan_parent_get_page;
    self->base.get_dbms = plan_update_get_dbms;
  }
  return self;

err:
  printf("plan_update_construct: Failed.\n");
  plan_parent_destruct(self);
  return self;
}
// }}}

// plan_delete

#undef VIRT_OVERRIDE
