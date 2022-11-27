#include "iterator.h"

#include <assert.h>
#include <malloc.h>

#include "core/meta.h"
#include "page.h"

// TESTING----------------
// Iterator on pages
typedef struct dp_page_iter {
  struct database_page *cur;
  fileoff_t cur_loc;
  // fileoff_t iend;
} dp_page_iter;

static void dp_page_iter_set(dp_page_iter *iter, database_page *cur) {
  iter->cur = cur;
  iter->cur_loc = cur->header.next;
}

static bool dp_page_iter_next(dp_page_iter *it, struct dbms *dbms) {
  fileoff_t next_loc = it->cur->header.next;
  dp_destruct(&it->cur);

  if (!fileoff_is_null(next_loc)) {
    dp_page_iter_set(it, dbms_dp_select(dbms, next_loc));
    return true;
  }
  return false;
}

// NOTE: NO NEED TO FREE PAGES, THEY ARE MANAGED BY ITERATOR
static dp_page_iter *dp_page_iter_construct(dbms *dbms) {
  dp_page_iter *iterator = my_malloc(dp_page_iter);

  fileoff_t page_loc = dbms->meta->dp.first;
  if (page_loc.bytes) {
    database_page *first = dbms_dp_select(dbms, page_loc);
    *iterator = (dp_page_iter){.cur = first, .cur_loc = page_loc};
  } else {
    *iterator = (dp_page_iter){.cur = NULL, .cur_loc = get_fileoff_t(0)};
  }
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
  database_page *page = dp_page_iter_get(iter->page_iter);
  iter->typle_iter = dp_typle_iter_construct(page);
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
      database_page *page = dp_page_iter_get(it->page_iter);
      it->typle_iter = dp_typle_iter_construct(page);

      if (dp_typle_iter_get(it->typle_iter) != NULL) {
        return true;
      }
    }
    // if no pages found create empty iterator
    it->typle_iter = dp_typle_iter_construct(NULL);
  }
  return false;
}

struct dp_typle *dp_iter_get(struct dp_iter *it) {
  return dp_typle_iter_get(it->typle_iter);
}

// Return current iterator page fileoff
fileoff_t dp_iter_cur_page(struct dp_iter *iter) { return iter->page_iter->cur_loc; }
// Return current iterator page offset
pageoff_t dp_iter_cur_index(struct dp_iter *iter) { return iter->typle_iter->icur; }
