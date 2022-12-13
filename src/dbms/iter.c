#include "iter.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "core/dbfile.h"
#include "core/dbmeta.h"
#include "io/p_table.h"
#include "op_schema.h"
#include "page.h"
#include "sso.h"
#include "table.h"
#include "table_dist.h"

// DATABASE PAGE {{{
typedef struct dp_page_iter {
  struct dbms *dbms;
  struct database_page *cur;
  fileoff_t cur_loc;
} dp_page_iter;

// read pages from last one to first
static bool dp_page_iter_next(dp_page_iter *it) {
  fileoff_t prev_loc = it->cur->header.prev;
  dp_destruct(&it->cur);

  if (!fileoff_is_null(prev_loc)) {
    it->cur = dbms_dp_open(it->dbms, prev_loc);
    it->cur_loc = prev_loc;
    return true;
  }
  return false;
}

static dp_page_iter *dp_page_iter_construct(dbms *dbms) {
  dp_page_iter *iterator = my_malloc(dp_page_iter);

  fileoff_t page_loc = dbms->meta->dp_last;
  if (!fileoff_is_null(page_loc)) {
    database_page *first = dbms_dp_open(dbms, page_loc);
    *iterator = (dp_page_iter){.cur = first, .cur_loc = page_loc, .dbms = dbms};
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
  {
    iter->page_iter = dp_page_iter_construct(dbms);

    database_page *page = dp_page_iter_get(iter->page_iter);
    iter->typle_iter = dp_tuple_iter_construct(page);
  }
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

    while (dp_page_iter_next(it->page_iter)) {
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
// }}}

// TABLE PAGE {{{
// tp_page_iter {{{
typedef struct tp_page_iter {
  struct dbms *dbms;
  struct dp_tuple *dpt;

  struct table_page *cur;
  fileoff_t cur_loc;

  bool do_write;
  bool was_full;// if was_full and not page is not full then add it in table_dist
} tp_page_iter;

static void tp_page_td_post(tp_page_iter *iter) {
  if (iter->was_full && !tp_is_full(iter->cur)) {
    page_entry entry = {.size = iter->cur->header.base.size, .start = iter->cur_loc};

    fileoff_t td_last = iter->dpt->header.td_last;
    dbms_td_push_single(iter->dbms, &td_last, &entry);
    iter->dpt->header.td_last = td_last;
  }
}

static void tp_page_iter_next_set(tp_page_iter *iter, table_page *cur,
                                  fileoff_t cur_loc) {
  iter->cur = cur;
  iter->cur_loc = cur_loc;
  iter->was_full = tp_is_full(cur);
}

static bool tp_page_iter_next(tp_page_iter *iter) {
  if (iter->do_write) {
    tp_page_td_post(iter);
    tp_alter(iter->cur, iter->dbms->dbfile->file, iter->cur_loc);
  }

  fileoff_t next_loc = iter->cur->header.next;

  tp_destruct(&iter->cur);

  if (!fileoff_is_null(next_loc)) {
    tp_page_iter_next_set(iter, dbms_tp_open(iter->dbms, next_loc), next_loc);
    return true;
  }
  return false;
}

// @dpt - is modified when is writable
static tp_page_iter *tp_page_iter_construct(struct dbms *dbms, dp_tuple *dpt,
                                            bool do_write) {
  assert(dpt->header.is_present && "it_page_iter: dp_tuple is not present.\n");

  tp_page_iter *iter = my_malloc(tp_page_iter);

  fileoff_t page_loc = dpt->header.tp_first;
  if (!fileoff_is_null(page_loc)) {
    table_page *first = dbms_tp_open(dbms, page_loc);
    *iter = (tp_page_iter){
        .dbms = dbms,
        .dpt = dpt,
        .cur = first,
        .cur_loc = page_loc,
        .do_write = do_write,
        .was_full = tp_is_full(first),
    };
  } else {
    *iter = (tp_page_iter){
        .cur = NULL,
        .cur_loc = get_fileoff_t(0),
        .was_full = false,
    };
  }
  return iter;
}

static void dpt_commit(struct dp_tuple *dpt, struct dbms *dbms) {
  const char *table_name = dbms_sso_construct_select(&dpt->header.sso, dbms);
  fileoff_t dpt_loc;
  pageoff_t dpt_off;
  bool res = dbms_find_table(table_name, dbms, &dpt_loc, &dpt_off);
  assert(res && "tp_page_iter: Table is not present in database.\n");

  database_page *dp;
  struct dp_tuple *old = dbms_select_tuple(dpt_loc, dpt_off, dbms, &dp);

  // check if columns are equals
  assert(old->header.cols == dpt->header.cols);
  for (size_t i = 0; i < old->header.cols; ++i) {
    assert(!memcmp(dpt->columns + i, old->columns + i, sizeof(dpt_column)));
  }
  // update header
  memcpy(&old->header, &dpt->header, sizeof(dpt_header));

  dbms_dp_close(&dp, dpt_loc, dbms);
  free((void *)table_name);
}

// @dbms - used if do_write == true
static void tp_page_iter_destruct(tp_page_iter **iter_ptr) {
  if (*iter_ptr == NULL)
    return;
  tp_page_iter *iter = *iter_ptr;
  if (iter->cur != NULL) {
    if (iter->do_write) {
      tp_page_td_post(iter);
      tp_alter(iter->cur, iter->dbms->dbfile->file, iter->cur_loc);
    }
    tp_destruct(&iter->cur);
  }
  // update
  if (iter->do_write) {
    dpt_commit(iter->dpt, iter->dbms);
  }
  free(iter);
  *iter_ptr = NULL;
}

static table_page *tp_page_iter_get(tp_page_iter *it) { return it->cur; }
//}}}

// tp_iter {{{
struct tp_iter *tp_iter_construct(struct dbms *dbms, dp_tuple *dpt, bool do_write) {

  assert(dpt->header.is_present && "tp_iter: dp_tuple should point to real page.\n");

  tp_iter *iter = my_malloc(tp_iter);
  *iter = (tp_iter){
      .page_iter = tp_page_iter_construct(dbms, dpt, do_write),
      .tuple_size = tp_get_tuple_size(dpt),
  };
  table_page *page = tp_page_iter_get(iter->page_iter);
  iter->tuple_iter = tp_tuple_iter_construct(page, iter->tuple_size);
  return iter;
}

void tp_iter_destruct(struct tp_iter **iter_ptr) {
  tp_iter *iter = *iter_ptr;
  if (iter == NULL)
    return;
  tp_tuple_iter_destruct(&iter->tuple_iter);
  tp_page_iter_destruct(&iter->page_iter);

  free(iter);
  *iter_ptr = NULL;
}

bool tp_iter_next(struct tp_iter *iter) {
  if (!tp_tuple_iter_next(iter->tuple_iter)) {
    tp_tuple_iter_destruct(&iter->tuple_iter);

    while (tp_page_iter_next(iter->page_iter)) {
      table_page *page = tp_page_iter_get(iter->page_iter);
      iter->tuple_iter = tp_tuple_iter_construct(page, iter->tuple_size);

      if (tp_tuple_iter_get(iter->tuple_iter) != NULL) {
        return true;
      }
    }
    iter->tuple_iter = tp_tuple_iter_construct(NULL, iter->tuple_size);
    return false;
  }
  return true;
}

// @return original tuple in case we want to update
struct tp_tuple *tp_iter_get(struct tp_iter *iter) {
  return tp_tuple_iter_get(iter->tuple_iter);
}

void tp_iter_update(struct tp_iter *iter, tp_tuple *tpt_new, tpt_col_info *info) {
  tp_tuple *tpt_old = tp_iter_get(iter);
  tpt_update(tpt_old, tpt_new, iter->page_iter->dpt, info, iter->page_iter->dbms);
}

void tp_iter_update_columns(struct tp_iter *iter, tp_tuple *tpt_new, tpt_col_info *info,
                            size_t arr_size, size_t *idxs) {
  tp_tuple *tpt_old = tp_iter_get(iter);
  tpt_update_columns(tpt_old, tpt_new, iter->page_iter->dpt, info, arr_size, idxs,
                     iter->page_iter->dbms);
}

void tp_iter_remove(struct tp_iter *iter, tpt_col_info *info) { 
  tp_tuple *tpt_old = tp_iter_get(iter);
  tpt_remove(tpt_old, iter->page_iter->dpt, info, iter->page_iter->dbms);
}

struct table_page *tp_iter_get_page(struct tp_iter *iter) {
  return tp_page_iter_get(iter->page_iter);
}

fileoff_t tp_iter_cur_page(struct tp_iter *iter) { return iter->page_iter->cur_loc; }
pageoff_t tp_iter_cur_tuple(struct tp_iter *iter) { return iter->tuple_iter->tcur; }
//}}}
//}}}
