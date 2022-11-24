#pragma once

#include <util/internals.h>

struct dbms;
struct database_page;

// Iterator on pages
typedef struct dp_page_iter {
  fileoff_t icur;
  fileoff_t iend;
} dp_page_iter;

// Iterator on every typle of every page
typedef struct dp_iter {
  struct dp_page_iter *page_iter;
  struct dp_typle_iter *typle_iter;
} dp_iter;

struct dp_page_iter dp_page_iterator(struct dbms *dbms);
bool dp_page_iter_next(struct dp_page_iter *it);
struct dp_typle *dp_page_iter_get(struct dp_page_iter *it);

struct dp_iter dp_iterator(struct dbms *dbms);
bool dp_iter_next(struct dp_iter *it);
struct dp_typle *dp_iter_get(struct dp_iter *it);
