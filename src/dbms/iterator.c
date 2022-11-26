#include "iterator.h"

#include <assert.h>
#include <malloc.h>

#include "core/dbfile.h"
#include "core/dbms.h"
#include "core/meta.h"
#include "io/database.h"
#include "io/page/database.h"
#include "page.h"

// Iterator on pages
typedef struct dp_page_iter {
  struct database_page *cur;
  fileoff_t next;
  // fileoff_t iend;
} dp_page_iter;

static void dp_page_iter_set(dp_page_iter *iter, database_page *cur) {
  iter->cur = cur;
  iter->next = cur != NULL ? cur->header.next : FILEOFF_NULL;
}

static bool dp_page_iter_next(dp_page_iter *it, struct dbms *dbms) {
  dp_destruct(&it->cur);

  if (!fileoff_is_null(it->next)) {
    dp_page_iter_set(it, dbms_dp_select(dbms, it->next));
    return true;
  }
  return false;
}

static dp_page_iter *dp_page_iter_construct(dbms *dbms) {
  dp_page_iter *iterator = my_malloc(dp_page_iter);

  fileoff_t page_start = dbms->meta->dp.first;
  *iterator = (dp_page_iter){.cur = NULL, .next = page_start};

  dp_page_iter_next(iterator, dbms);
  return iterator;
}

// if iterator doesn't iterate to the end
static void dp_page_iter_destruct(dp_page_iter **iter_ptr) {
  if (*iter_ptr == NULL)
    return;

  dp_page_iter *iter = *iter_ptr;
  if (iter->cur != NULL) {
    dp_destruct(&iter->cur);
  }
  free(iter);
  *iter_ptr = NULL;
}

static database_page *dp_page_iter_get(dp_page_iter *it) { return it->cur; }

// Iterator on every typle of page
struct dp_iter *dp_iter_construct(struct dbms *dbms) {
  dp_iter *iter = my_malloc(dp_iter);
  iter->page_iter = dp_page_iter_construct(dbms);
  iter->typle_iter = dp_typle_iter_construct(dp_page_iter_get(iter->page_iter));
  return iter;
}

void dp_iter_destruct(struct dp_iter **iter_ptr) {
  dp_iter *iter = *iter_ptr;
  if (iter == NULL) {
    return;
  }
  dp_typle_iter_destruct(&iter->typle_iter);
  dp_page_iter_destruct(&iter->page_iter);
  free(iter);
  *iter_ptr = NULL;
}

bool dp_iter_next(struct dp_iter *it, struct dbms *dbms) {
  if (!dp_typle_iter_next(it->typle_iter)) {

    dp_typle_iter_destruct(&it->typle_iter);

    while (dp_page_iter_next(it->page_iter, dbms)) {
      it->typle_iter = dp_typle_iter_construct(dp_page_iter_get(it->page_iter));

      if (dp_typle_iter_get(it->typle_iter) != NULL) {
        return true;
      }
    }
  }
  return false;
}

struct dp_typle *dp_iter_get(struct dp_iter *it) {
  return dp_typle_iter_get(it->typle_iter);
}
