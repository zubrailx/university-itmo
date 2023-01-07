#include "plan_filter.h"
#include "dbms/io/page/p_database.h"
#include "dbms/io/page/p_table.h"
#include "dbms/op_table.h"
#include "dbms/table.h"

#include <assert.h>
#include <dbms/plan.h>
#include <dbms/plan_funcs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fast {{{
struct fast fast_construct(enum fast_type type) { return (struct fast){.type = type}; }

void fast_destruct(void *self_void) {
  struct fast *self = self_void;
  self->type = FAST_FREED;
  if (self->res) {
    free(self->res);
    self->res = NULL;
  }
  free(self);
}
// }}}

// fast_const {{{
static void fast_const_compile(void *self_void, size_t pti_size,
                               struct plan_table_info *info_arr) {}

static void *fast_const_calc(void *self_void) {
  struct fast *self = self_void;
  return self->res;
}

static void fast_const_pass(void *self, const struct tp_tuple **tuple_arr) {}

struct fast_const *fast_const_construct(enum table_column_type col_type,
                                        const void *value, struct dbms *dbms) {
  struct fast_const *self = my_malloc(struct fast_const);
  *self = (struct fast_const){
      .dbms = dbms,
  };

  self->base = fast_construct(FAST_CONST);
  self->base.res_type = col_type;

  // Set tpt_col
  size_t col_size = tp_column_size(col_type);
  self->base.res = calloc(col_size, 1);
  if (value == NULL) {
    ((struct tpt_column_base *)self->base.res)->is_null = true;
  } else {
    tpt_memcpy_specific(self->base.res, value, self->base.res_type, self->dbms);
  }

  self->base.compile = fast_const_compile;
  self->base.destruct = fast_destruct;
  self->base.calc = fast_const_calc;
  self->base.pass = fast_const_pass;
  return self;
}
//}}}

// fast_column {{{
static void fast_column_compile(void *self_void, size_t pti_size,
                                struct plan_table_info *info_arr) {
  struct fast_column *self = self_void;

  for (size_t ti = 0; ti < pti_size; ++ti) {
    const struct plan_table_info *pti = info_arr + ti;
    if (!strcmp(pti->table_name, self->table_name)) {
      int ci = dbms_find_column_idx(pti->dpt, self->column_name, self->dbms);
      if (ci < 0) {
        printf("Column not found in table '%s'.\n", self->table_name);
        assert(0 && "Not found.\n");
      }
      // Set idxs, allocate tptc
      self->tbl_idx = ti;

      tpt_col_info *col_info = tp_construct_col_info_arr(pti->dpt);
      self->resc_off = col_info[ci].start;
      free(col_info);

      self->base.res_type = pti->dpt->columns[ci].type;
      self->resc_size = tp_column_size(self->base.res_type);

      if (self->base.res) {
        free(self->base.res);
      }
      self->base.res = malloc(self->resc_size);
    }
  }
  if (self->resc_size == 0) {
    assert(0 && "fast_column: Table or column not found.\n");
    return;
  }
}

static void fast_column_destruct(void *self_void) {
  struct fast_column *self = self_void;
  free(self->table_name);
  free(self->column_name);
  fast_destruct(self);
}

static void *fast_column_calc(void *self_void) {
  struct fast_column *self = self_void;
  return self->base.res;
}

static void fast_column_pass(void *self_void, const struct tp_tuple **tuple_arr) {
  struct fast_column *self = self_void;

  void *tptc_addr = (void *)tuple_arr[self->tbl_idx] + self->resc_off;
  memcpy(self->base.res, tptc_addr, self->resc_size);
}

struct fast_column *fast_column_construct(const char *table_name,
                                          const char *column_name, struct dbms *dbms) {
  struct fast_column *self = my_malloc(struct fast_column);
  *self = (struct fast_column){
      .base = fast_construct(FAST_COLUMN),
      .table_name = strdup(table_name),
      .column_name = strdup(column_name),
      .dbms = dbms,
  };

  self->base.compile = fast_column_compile;
  self->base.destruct = fast_column_destruct;
  self->base.calc = fast_column_calc;
  self->base.pass = fast_column_pass;
  return self;
}
//}}}

// fast_unop {{{
static void fast_unop_compile(void *self_void, size_t pti_size,
                              struct plan_table_info *info_arr) {
  struct fast_unop *self = self_void;
  self->parent->compile(self->parent, pti_size, info_arr);
}

static void fast_unop_destruct(void *self_void) {
  struct fast_unop *self = self_void;
  self->parent->destruct(self->parent);
  fast_destruct(self);
}

static void *fast_unop_calc(void *self_void) {
  struct fast_unop *self = self_void;
  void *arg = self->parent->calc(self->parent);
  self->func(self, arg);
  return self->base.res;
}

static void fast_unop_pass(void *self_void, const struct tp_tuple **tuple_arr) {
  struct fast_unop *self = self_void;
  self->parent->pass(self->parent, tuple_arr);
}

struct fast_unop *fast_unop_construct(void *parent, const struct fast_unop_func *fuf,
                                      struct dbms *dbms) {
  struct fast_unop *self = my_malloc(struct fast_unop);
  *self = (struct fast_unop){
      .base = fast_construct(FAST_UNOP),
      .parent = parent,
      .dbms = dbms,
  };

  self->base.res_type = fuf->ret_type;
  self->base.res = calloc(tp_column_size(fuf->ret_type), 1);

  self->base.compile = fast_unop_compile;
  self->base.destruct = fast_unop_destruct;
  self->base.calc = fast_unop_calc;
  self->base.pass = fast_unop_pass;

  self->func = fuf->func;
  return self;
}
//}}}

// fast_binop {{{
static void fast_binop_compile(void *self_void, size_t pti_size,
                               struct plan_table_info *info_arr) {
  struct fast_binop *self = self_void;
  self->left->compile(self->left, pti_size, info_arr);
  self->right->compile(self->right, pti_size, info_arr);
}

static void fast_binop_destruct(void *self_void) {
  struct fast_binop *self = self_void;
  self->left->destruct(self->left);
  self->right->destruct(self->right);
  fast_destruct(self);
}

static void *fast_binop_calc(void *self_void) {
  struct fast_binop *self = self_void;
  void *arg1 = self->left->calc(self->left);
  void *arg2 = self->right->calc(self->right);
  self->func(self, arg1, arg2);
  return self->base.res;
}

static void fast_binop_pass(void *self_void, const struct tp_tuple **tuple_arr) {
  struct fast_binop *self = self_void;

  self->left->pass(self->left, tuple_arr);
  self->right->pass(self->right, tuple_arr);
}

struct fast_binop *fast_binop_construct(void *p_left, void *p_right,
                                        const struct fast_binop_func *fbf,
                                        struct dbms *dbms) {
  struct fast_binop *self = my_malloc(struct fast_binop);
  *self = (struct fast_binop){
      .base = fast_construct(FAST_BINOP),
      .left = p_left,
      .right = p_right,
      .dbms = dbms,
  };

  self->base.res_type = fbf->ret_type;
  self->base.res = calloc(tp_column_size(fbf->ret_type), 1);

  self->base.compile = fast_binop_compile;
  self->base.destruct = fast_binop_destruct;
  self->base.calc = fast_binop_calc;
  self->base.pass = fast_binop_pass;

  self->func = fbf->func;
  return self;
}
//}}}
