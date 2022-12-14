#include "plan_filter.h"
#include "converters/table.h"
#include "io/page/p_table.h"
#include <stdlib.h>
#include <string.h>
#include <util/define.h>

// fast {{{
struct fast fast_construct(enum fast_type type) { return (struct fast){.type = type}; }
// }}}

// fast_const {{{
static void fast_const_compile(void *self_void, struct plan_table_info *info_arr) {}

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

//}}}

// fast_unop {{{

//}}}

// fast_binop {{{

//}}}
