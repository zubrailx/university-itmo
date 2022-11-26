#pragma once

#include <util/internals.h>

struct dbms;

// Iterator on every typle of every page
typedef struct dp_iter {
  struct dp_page_iter *page_iter;
  struct dp_typle_iter *typle_iter;
} dp_iter;

struct dp_iter *dp_iter_construct(struct dbms *dbms);
void dp_iter_destruct(struct dp_iter **iter_ptr);
bool dp_iter_next(struct dp_iter *it, struct dbms *dbms);
struct dp_typle *dp_iter_get(struct dp_iter *it);
void dp_iter_destruct(struct dp_iter **it_ptr);
