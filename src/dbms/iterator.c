#include "iterator.h"

#include <assert.h>

#include "core/dbfile.h"
#include "core/dbms.h"
#include "core/meta.h"
#include "io/database.h"
#include "io/page/database.h"

// Iterator on pages
typedef struct dp_page_iter {
  struct database_page *cur;
  // fileoff_t icur;
  // fileoff_t iend;
} dp_page_iter;

static struct dp_page_iter dp_page_iterator(struct dbms *dbms) {
  fileoff_t page_start = dbms->meta->dp.first;
  FILE *file = dbms->dbfile->file;
  assert(!fileoff_is_null(page_start));

  pageoff_t page_size;
  page_load_size(&page_size, file, page_start);
  struct database_page *dp = dp_construct(page_size);
  dp_load(dp, file, page_start);
}
static bool dp_page_iter_next(struct dp_page_iter *it) {}
static struct dp_typle *dp_page_iter_get(struct dp_page_iter *it) {}

// Iterator on every typle of page
struct dp_iter dp_iterator(struct dbms *dbms) {}
bool dp_iter_next(struct dp_iter *it) {}
struct dp_typle *dp_iter_get(struct dp_iter *it) {}
