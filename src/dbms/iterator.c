#include "iterator.h"

#include "core/dbms.h"

// Iterator on pages
typedef struct dp_page_iter {
  fileoff_t icur;
  fileoff_t iend;
} dp_page_iter;

static struct dp_page_iter dp_page_iterator(struct dbms *dbms) {}
static bool dp_page_iter_next(struct dp_page_iter *it) {}
static struct dp_typle *dp_page_iter_get(struct dp_page_iter *it) {}

// Iterator on every typle of page
struct dp_iter dp_iterator(struct dbms *dbms) {}
bool dp_iter_next(struct dp_iter *it) {}
struct dp_typle *dp_iter_get(struct dp_iter *it) {}
