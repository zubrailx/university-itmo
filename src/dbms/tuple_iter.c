#include "tuple_iter.h"

#include <assert.h>
#include <malloc.h>

#include "core/dbmeta.h"
#include "page.h"

// DATABASE PAGE
typedef struct dp_page_iter {
  struct database_page *cur;
  fileoff_t cur_loc;
} dp_page_iter;

// read pages from last one to first
static bool dp_page_iter_next(dp_page_iter *it, struct dbms *dbms) {
  fileoff_t prev_loc = it->cur->header.prev;
  dp_destruct(&it->cur);

  if (!fileoff_is_null(prev_loc)) {
    it->cur = dbms_dp_open(dbms, prev_loc);
    it->cur_loc = prev_loc;
    return true;
  }
  return false;
}
// NOTE: NO NEED TO FREE PAGES, THEY ARE MANAGED BY ITERATOR
static dp_page_iter *dp_page_iter_construct(dbms *dbms) {
  dp_page_iter *iterator = my_malloc(dp_page_iter);

  fileoff_t page_loc = dbms->meta->dp_last;
  if (!fileoff_is_null(page_loc)) {
    database_page *first = dbms_dp_open(dbms, page_loc);
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
  iter->typle_iter = dp_tuple_iter_construct(page);
  iter->dbms = dbms;
  return iter;
}

void dp_iter_destruct(struct dp_iter **iter_ptr) {
  dp_iter *iter = *iter_ptr;
  if (iter == NULL) {
    return;
  }
  dp_tuple_iter_destruct(&iter->typle_iter);
  dp_page_iter_destruct(&iter->page_iter);
  free(iter);
  *iter_ptr = NULL;
}

bool dp_iter_next(struct dp_iter *it) {
  if (!dp_tuple_iter_next(it->typle_iter)) {

    dp_tuple_iter_destruct(&it->typle_iter);

    while (dp_page_iter_next(it->page_iter, it->dbms)) {
      database_page *page = dp_page_iter_get(it->page_iter);
      it->typle_iter = dp_tuple_iter_construct(page);

      if (dp_tuple_iter_get(it->typle_iter) != NULL) {
        return true;
      }
    }
    // if no pages found create empty iterator
    it->typle_iter = dp_tuple_iter_construct(NULL);
  }
  return false;
}

struct dp_tuple *dp_iter_get(struct dp_iter *it) {
  return dp_tuple_iter_get(it->typle_iter);
}

// Return current iterator page fileoff
fileoff_t dp_iter_cur_page(struct dp_iter *iter) { return iter->page_iter->cur_loc; }
// Return current iterator page offset
pageoff_t dp_iter_cur_index(struct dp_iter *iter) { return iter->typle_iter->icur; }

// TABLE PAGE
typedef struct tp_page_iter {
  struct table_page *cur;
  fileoff_t cur_loc;
} tp_page_iter;

static void tp_page_iter_set(tp_page_iter *iter, table_page *cur) {
  iter->cur = cur;
  iter->cur_loc = cur->header.next;
}

static bool tp_page_iter_next(tp_page_iter *it, struct dbms *dbms) {
  fileoff_t next_loc = it->cur->header.next;
  tp_destruct(&it->cur);

  if (!fileoff_is_null(next_loc)) {
    tp_page_iter_set(it, dbms_tp_open(dbms, next_loc));
    return true;
  }
  return false;
}

static tp_page_iter *tp_page_iter_construct(struct dbms *dbms, const dp_tuple *tuple) {
  tp_page_iter *iter = my_malloc(tp_page_iter);

  fileoff_t page_loc = tuple->header.tp_first;
  if (!fileoff_is_null(page_loc)) {
    table_page *first = dbms_tp_open(dbms, page_loc);
    *iter = (tp_page_iter){.cur = first, .cur_loc = page_loc};
  } else {
    *iter = (tp_page_iter){.cur = NULL, .cur_loc = get_fileoff_t(0)};
  }
  return iter;
}

static void tp_page_iter_destruct(tp_page_iter **iter_ptr) {
  if (*iter_ptr == NULL)
    return;
  tp_page_iter *iter = *iter_ptr;
  if (iter->cur != NULL) {
    tp_destruct(&iter->cur);
  }
  free(iter);
  *iter_ptr = NULL;
}

static table_page *tp_page_iter_get(tp_page_iter *it) { return it->cur; }

// Typle iterator
struct tp_iter *tp_iter_construct(struct dbms *dbms, const dp_tuple *typle) {
  tp_iter *iter = my_malloc(tp_iter);
  *iter = (tp_iter){
      .dbms = dbms,
      .page_iter = tp_page_iter_construct(dbms, typle),
      .typle_size = tp_get_tuple_size(typle),
  };
  table_page *page = tp_page_iter_get(iter->page_iter);
  iter->typle_iter = tp_tuple_iter_construct(page, iter->typle_size);
  return iter;
}

void tp_iter_destruct(struct tp_iter **iter_ptr) {
  tp_iter *iter = *iter_ptr;
  if (iter == NULL)
    return;
  tp_tuple_iter_destruct(&iter->typle_iter);
  tp_page_iter_destruct(&iter->page_iter);
  free(iter);
  *iter_ptr = NULL;
}

bool tp_iter_next(struct tp_iter *it) {
  if (!tp_tuple_iter_next(it->typle_iter)) {
    tp_tuple_iter_destruct(&it->typle_iter);

    while (tp_page_iter_next(it->page_iter, it->dbms)) {
      table_page *page = tp_page_iter_get(it->page_iter);
      it->typle_iter = tp_tuple_iter_construct(page, it->typle_size);

      if (tp_tuple_iter_get(it->typle_iter) != NULL) {
        return true;
      }
    }
    it->typle_iter = tp_tuple_iter_construct(NULL, it->typle_size);
  }
  return false;
}

struct tp_tuple *tp_iter_get(struct tp_iter *iter) {
  return tp_tuple_iter_get(iter->typle_iter);
}

fileoff_t tp_iter_cur_page(struct tp_iter *iter) { return iter->page_iter->cur_loc; }
pageoff_t tp_iter_cur_typle(struct tp_iter *iter) { return iter->typle_iter->tcur; }
