#pragma once

#include <util/internals.h>

// TODO: implement when whole system will work correctly
// (DONT USE IT CURRENTLY, NIKITA, DONT BE STUPID)
struct dbms;

// Iterator on every typle of every page
typedef struct dp_iter {
  struct dp_page_iter *page_iter;
  struct dp_typle_iter *typle_iter;
} dp_iter;

struct dp_iter dp_iterator(struct dbms *dbms);
bool dp_iter_next(struct dp_iter *it);
struct dp_typle *dp_iter_get(struct dp_iter *it);
