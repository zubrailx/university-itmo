#include "plan_filter.h"
#include "converters/table.h"
#include "io/page/p_table.h"
#include "op_table.h"
#include "plan.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/define.h>

// fast {{{
struct fast fast_construct(enum fast_type type) { return (struct fast){.type = type}; }
// }}}

// fast_const {{{
static void fast_const_compile(void *self_void, size_t pti_size,
                               struct plan_table_info *info_arr) {}

static void fast_const_destruct(void *self_void) {
  struct fast_const *self = self_void;
  free(self->tpt_col);
  self->base.type = FAST_FREED;
  free(self);
}

static void *fast_const_calc(void *self_void) {
  struct fast_const *self = self_void;
  return self->tpt_col;
}

static void fast_const_pass(void *self, const struct tp_tuple **tuple_arr) {}

struct fast_const *fast_const_construct(enum dto_table_column_type col_type,
                                        void *value) {
  struct fast_const *self = my_malloc(struct fast_const);
  *self = (struct fast_const){};

  self->base = fast_construct(FAST_CONST);

  self->tpt_type = column_type_to_page(col_type);
  // Set tpt_col
  size_t col_size = tp_column_size(col_type);
  size_t entry_size = tp_entry_size(col_type);
  self->tpt_col = calloc(col_size, 1);
  if (value == NULL) {
    self->tpt_col->is_null = true;
  } else {
    memcpy((void *)self->tpt_col + (col_size - entry_size), value, entry_size);
  }

  self->base.compile = fast_const_compile;
  self->base.destruct = fast_const_destruct;
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
      self->tptc_off = col_info[ci].start;
      free(col_info);

      self->tptc_type = pti->dpt->columns[ci].type;
      self->tptc_size = tp_column_size(self->tptc_type);

      if (self->tptc) {
        free(self->tptc);
      }
      self->tptc = malloc(self->tptc_size);
    }
  }
  if (self->tptc_size == 0) {
    assert(0 && "fast_column: Table or column not found.\n");
    return;
  }
}

static void fast_column_destruct(void *self_void) {
  struct fast_column *self = self_void;
  free(self->table_name);
  free(self->column_name);
  free(self->tptc);
  self->base.type = FAST_FREED;
  free(self);
}

static void *fast_column_calc(void *self_void) {
  struct fast_column *self = self_void;
  return self->tptc;
}

static void fast_column_pass(void *self_void, const struct tp_tuple **tuple_arr) {
  struct fast_column *self = self_void;

  void *tptc_addr = (void *)tuple_arr[self->tbl_idx] + self->tptc_off;
  memcpy(self->tptc, tptc_addr, self->tptc_size);
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

//}}}

// fast_binop {{{

//}}}
