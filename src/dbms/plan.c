#include "plan.h"
#include "io/page/p_table.h"
#include "iter.h"
#include "op_schema.h"
#include "page.h"
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

struct plan plan_construct(enum plan_type type) {
  return (struct plan){.type = type,
                       .arr_size = 0,
                       .pti_arr = NULL,
                       .tuple_arr = NULL,
                       // methods
                       .get_info = plan_get_info,
                       .get = plan_get,
                       .next = plan_next,
                       .end = plan_end,
                       .destruct = plan_destruct_void,
                       .locate = plan_locate};
}

struct plan plan_construct_with_row_info(enum plan_type type, const size_t size,
                                         struct plan_table_info arr[]) {
  struct plan plan = plan_construct(type);

  plan.arr_size = size;

  size_t info_size = sizeof(struct plan_table_info) * size;
  plan.pti_arr = malloc(info_size);
  memcpy(plan.pti_arr, arr, info_size);

  plan.tuple_arr = malloc(sizeof(tp_tuple *) * size);

  return plan;
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

  {
    database_page *dp;
    const struct dp_tuple *dpt = dbms_select_tuple(dpt_loc, dpt_off, dbms, &dp);

    size_t dpt_size = dp_tuple_size(dpt->header.cols);
    plan_dpt = malloc(dpt_size);
    memcpy(plan_dpt, dpt, dpt_size);

    dp_destruct(&dp);
  }

  self->iter = tp_iter_construct(dbms, plan_dpt);

  {// set base structure field
    struct plan_table_info table_nfo = {
        .table_name = strdup(table_name),
        .dpt = plan_dpt,
        .tpt_size = dp_tuple_size(plan_dpt->header.cols),
        .col_info = tp_construct_col_info_arr(plan_dpt)};

    self->base = plan_construct_with_row_info(PLAN_TYPE_SOURCE, 1, &table_nfo);
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

    (*self).locate = plan_source_locate;
    VIRT_OVERRIDE((*self), base, locate);
  }

  return self;
err:
  printf("plan_source_construct: Failed.\n");
  plan_source_destruct((void **)&self);
  return self;
}
// }}}

// plan_select {{{
static void plan_select_destruct(void *self_void) {
  struct plan_select *self = self_void;
  // free parent node
  if (self->parent) {
    self->parent->destruct(self->parent);
  }
  // call base destructor
  plan_destruct(&self->base);
}

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

  bool res = parent->next(self->parent);
  if (!res) {
    return false;
  }
  struct tp_tuple **tuple_src = parent->get(self->parent);
  struct tp_tuple *tuple_dest = self->base.tuple_arr[0];

  tpt_flatten(tuple_dest, tuple_src, parent);

  return true;
}

static bool plan_select_end(void *self_void) {
  struct plan_select *self = self_void;
  return self->parent->end(self->parent);
}

static dp_tuple *dpt_flatten(size_t size, const struct plan_table_info *src) {
  struct dp_tuple *dest;

  size_t total_cols = 0;
  for (size_t i = 0; i < size; ++i) {
    // calculate total amount of columns to create dp_tuple
    total_cols += src[i].dpt->header.cols;
  }

  {
    dest = malloc(dp_tuple_size(total_cols));
    // header
    // zero everything until columns
    memset(dest, 0, offsetof(struct dp_tuple, columns));
    dest->header.cols = total_cols;
    dest->header.is_present = false;
    // Columns
    size_t cur_col = 0;
    for (size_t i = 0; i < size; ++i) {
      const size_t icol_size = src[i].tpt_size - offsetof(struct dp_tuple, columns);
      memcpy(dest->columns + cur_col, src[i].dpt->columns, icol_size);

      cur_col += src[i].dpt->header.cols;
    }
  }
  return dest;
}

// @parent_void - moved
struct plan_select *plan_select_construct_move(void *parent_void,
                                               const char *table_name) {
  struct plan_select *self = my_malloc(struct plan_select);

  struct plan *parent = parent_void;
  parent_void = NULL;

  self->parent = parent;

  {// base
    const struct plan_table_info *parent_info = parent->get_info(parent);

    struct dp_tuple *dpt = dpt_flatten(parent->arr_size, parent_info);

    struct plan_table_info table_info = {.table_name = strdup(table_name),
                                         .dpt = dpt,
                                         .tpt_size = dp_tuple_size(dpt->header.cols),
                                         .col_info = tp_construct_col_info_arr(dpt)};

    self->base = plan_construct_with_row_info(PLAN_TYPE_SELECT, 1, &table_info);

    // allocate tuple_arr[0] and set to initial value
    self->base.tuple_arr[0] = malloc(self->base.pti_arr[0].tpt_size);
    tpt_flatten(self->base.tuple_arr[0], parent->get(parent), parent);
  }

  {
    VIRT_INHERIT((*self), base, get);
    VIRT_INHERIT((*self), base, get_info);
    VIRT_INHERIT((*self), base, locate);

    self->next = plan_select_next;
    VIRT_OVERRIDE((*self), base, next);

    self->end = plan_select_end;
    VIRT_OVERRIDE((*self), base, end);

    self->destruct = plan_select_destruct;
    VIRT_OVERRIDE((*self), base, destruct);
  }

  return self;
}

// }}}

#undef VIRT_OVERRIDE
