#pragma once

#include "database.h"
#include "typle.h"

my_defstruct(dp_typle_iterator);
struct dp_typle_iterator {
  struct database_page *page;
  pageoff_t icur;
  pageoff_t iend; // points after last element in list
};

struct dp_typle_iterator dp_get_typle_iterator(struct database_page *page);
bool dp_typle_iterator_next(struct dp_typle_iterator *it);
struct dp_typle *dp_typle_iterator_get(struct dp_typle_iterator *it);
