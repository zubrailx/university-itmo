#include "table_dist.h"

#include "core/dbfile.h"
#include "core/dbms.h"
#include "io/page/p_container.h"
#include "io/page/p_database.h"
#include "io/page/p_table.h"
#include "page.h"
#include "pagealloc.h"

static pageoff_t tp_size(const dp_tuple *tuple) {
  pageoff_t min_size = tp_get_min_size(tuple);
  if (TABLE_PAGE_MIN_SIZE.bytes > min_size.bytes) {
    return TABLE_PAGE_MIN_SIZE;
  } else {
    return min_size;
  }
}

fileoff_t dbms_td_create_close(struct dbms *dbms) {
  return dbms_container_create_close(dbms, FILEOFF_NULL, SIZE_DEFAULT);
}

void dbms_td_drop(struct dbms *dbms, const fileoff_t last) {
  fileoff_t cur_loc = last;

  page_entry entry;
  page_container *cur;

  while (fileoff_is_null(cur_loc)) {
    cur = dbms_container_open(dbms, cur_loc);
    entry.start = cur_loc;
    entry.size = cur->header.base.size;

    cur_loc = cur->header.prev;

    container_destruct(&cur);
    dbms_page_free(dbms, &entry);
  }
}

// pop last entry or malloc it
// @return - bool == false if malloc else true
bool dbms_td_pop_single(struct dbms *dbms, const dp_tuple *tuple, fileoff_t *gappy_last,
                       page_entry *pe_out) {
  page_container *last = dbms_container_open(dbms, *gappy_last);

  const page_entry last_entry = {.size = last->header.base.size, .start = *gappy_last};
  const fileoff_t prev_loc = last->header.prev;

  if (container_empty(last)) {

    container_destruct(&last);

    if (!fileoff_is_null(prev_loc)) {
      *gappy_last = prev_loc;
      page_container *prev = dbms_container_open(dbms, prev_loc);

      *pe_out = *container_pop(prev);
      // write and close resources
      dbms_container_close(&prev, prev_loc, dbms);

      dbms_page_free(dbms, &last_entry);
    } else {
      // use malloc for faster allocations
      *pe_out = dbms_page_malloc(dbms, tp_size(tuple));
      return false;
    }
  } else {
    *pe_out = *container_pop(last);
    dbms_container_close(&last, last_entry.start, dbms);
  }
  return true;
}

void dbms_td_push_single(struct dbms *dbms, fileoff_t *gappy_last,
                        const page_entry *entry) {
  page_container *last = dbms_container_open(dbms, *gappy_last);

  if (container_full(last)) {
    container_destruct(&last);
    *gappy_last = dbms_container_create(dbms, SIZE_DEFAULT, *gappy_last, &last);
  }
  container_push(last, entry);
  dbms_container_close(&last, *gappy_last, dbms);
}
