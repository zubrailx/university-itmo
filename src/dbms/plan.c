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

// base plan
static const struct plan_row_info *plan_get_info(void *self) {
  return ((struct plan *)self)->info_arr;
}
static struct tp_tuple **plan_get(void *self) { return NULL; }

static bool plan_next(void *self) { return false; }

static bool plan_end(void *self) { return true; }

static void plan_destruct(void *self_void) {
  struct plan *self = self_void;
  self->type = PLAN_TYPE_DELETED;
  // free row info
  if (self->info_arr) {
    for (int i = 0; i < self->arr_size; ++i) {
      struct plan_row_info *ie = self->info_arr + i;
      if (ie->table_name) {
        free((void *)ie->table_name);
      }
      if (ie->info) {
        free(ie->info);
      }
    }
  }
  // free tuple_arr
  if (self->tuple_arr) {
    free(self->tuple_arr);
  }
}

struct plan plan_construct(enum plan_type type) {
  return (struct plan){
      .type = type,
      .arr_size = 0,
      .info_arr = NULL,
      .tuple_arr = NULL,
      .get_info = plan_get_info,
      .get = plan_get,
      .next = plan_next,
      .end = plan_end,
      .destruct = plan_destruct,
  };
}

struct plan plan_construct_with_row_info(enum plan_type type, const size_t size,
                                         struct plan_row_info arr[]) {
  struct plan plan = plan_construct(type);

  plan.arr_size = size;

  size_t info_size = sizeof(struct plan_row_info) * size;
  plan.info_arr = malloc(info_size);
  memcpy(plan.info_arr, arr, info_size);

  plan.tuple_arr = malloc(sizeof(tp_tuple *) * size);

  return plan;
}

// plan_source
static void plan_source_destruct(void *self_void) {
  struct plan_source *self = self_void;
  if (self->iter) {
    tp_iter_destruct(&self->iter);
  }
  if (self->dp) {
    dbms_dp_close(&self->dp, self->dp_loc, self->dbms);
  }
  // call base destructor
  plan_destruct(&self->base);
}

static tp_tuple **plan_source_get(void *self_void) {
  struct plan_source *self = self_void;
  // tuple_arr is net in next
  return self->base.tuple_arr;
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

struct plan_source plan_source_construct(const void *table_name, struct dbms *dbms) {
  struct plan_source self = {};
  // specific for source
  self.dbms = dbms;
  {// dpt, dp
    pageoff_t off;
    if (!dbms_find_table(table_name, dbms, &self.dp_loc, &off)) {
      printf("plan_source_construct: Table not found.\n");
      self.dp_loc = FILEOFF_NULL;
      goto err;
    }
    self.dpt = dbms_select_tuple(self.dp_loc, off, dbms, &self.dp);
  }
  self.iter = tp_iter_construct(dbms, self.dpt);

  {// set base structure field
    struct plan_row_info row_info = {.table_name = strdup(table_name),
                                     .info = tp_construct_col_info_arr(self.dpt)};
    self.base = plan_construct_with_row_info(PLAN_TYPE_SOURCE, 1, &row_info);
    self.base.tuple_arr[0] = tp_iter_get(self.iter);
  }
  {
    // set methods
    VIRT_INHERIT(self, base, get_info);
    self.next = plan_source_next;
    self.end = plan_source_end;
    self.destruct = plan_source_destruct;
    self.get = plan_source_get;

    VIRT_OVERRIDE(self, base, get);
    VIRT_OVERRIDE(self, base, next);
    VIRT_OVERRIDE(self, base, end);
    VIRT_OVERRIDE(self, base, destruct);
  }

  return self;
err:
  printf("plan_source_construct: Failed.\n");
  plan_source_destruct(&self);
  return self;
}

#undef VIRT_OVERRIDE
