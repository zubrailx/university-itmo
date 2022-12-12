#pragma once

#include "io/page/p_table.h"
#include <stddef.h>
#include <util/internals.h>

struct dbms;

// Database Page
typedef struct dp_iter {
  struct dbms *dbms;
  struct dp_page_iter *page_iter;
  struct dp_tuple_iter *typle_iter;
} dp_iter;

struct dp_iter *dp_iter_construct(struct dbms *dbms);
void dp_iter_destruct(struct dp_iter **iter_ptr);
bool dp_iter_next(struct dp_iter *it);
struct dp_tuple *dp_iter_get(struct dp_iter *iter);

fileoff_t dp_iter_cur_page(struct dp_iter *iter);
pageoff_t dp_iter_cur_index(struct dp_iter *iter);

// Table Page
typedef struct tp_iter {
  struct dbms *dbms;

  size_t typle_size;
  bool do_write;

  struct tp_page_iter *page_iter;
  struct tp_tuple_iter *tuple_iter;
} tp_iter;

struct tp_iter *tp_iter_construct(struct dbms *dbms, struct dp_tuple *dpt,
                                  bool do_write);
void tp_iter_destruct(struct tp_iter **iter_ptr);
bool tp_iter_next(struct tp_iter *iter);
struct tp_tuple *tp_iter_get(struct tp_iter *iter);
struct table_page *tp_iter_get_page(struct tp_iter *iter);

fileoff_t tp_iter_cur_page(struct tp_iter *iter);
pageoff_t tp_iter_cur_tuple(struct tp_iter *iter);
