#include "pagealloc.h"

#include "core/dbfile.h"
#include "core/dbms.h"
#include "io/meta/meta.h"
#include "io/p_container.h"
#include "page.h"
#include <assert.h>

const pageoff_t PAGEPOOL_PAGE_SIZE = (pageoff_t){.bytes = 1024};

// create last page for pagepool meta->free_last is managed automatically
// @prev - previous page
static fileoff_t dbms_pa_create_append(struct dbms *dbms, const fileoff_t prev,
                                             page_container **pc_ptr) {
  FILE *file = dbms->dbfile->file;
  meta *meta = dbms->meta;
  pageoff_t size = PAGEPOOL_PAGE_SIZE;

  page_container *pc = container_construct_init(size, prev);
  fileoff_t pc_loc = meta->pos_empty;
  container_create(pc, file, meta->pos_empty);

  meta->pos_empty.bytes += size.bytes;
  *pc_ptr = pc;

  // update free_last
  meta->free_last = pc_loc;
  return pc_loc;
}

// first of all create page container
void dbms_pa_create_close(struct dbms *dbms, const fileoff_t prev) {
  page_container *pc;
  dbms_pa_create_append(dbms, prev, &pc);
  container_destruct(&pc);
}

static page_container *dbms_pa_select(const struct dbms *dbms,
                                            const fileoff_t pc_loc) {
  FILE *file = dbms->dbfile->file;
  pageoff_t size;
  page_load_size(&size, file, pc_loc);
  page_container *pc = container_construct(size);
  container_load(pc, file, pc_loc);
  return pc;
}

static page_entry dbms_pa_alloc(struct dbms *dbms, pageoff_t size) {
  base_page *page = page_construct(size, PAGE_UNKNOWN);
  page_create(page, dbms->dbfile->file, dbms->meta->pos_empty);
  page_entry entry =
      (page_entry){.size = page->header.size, .start = dbms->meta->pos_empty};

  dbms->meta->pos_empty.bytes += page->header.size.bytes;
  page_destruct(&page);
  return entry;
}

/*
 * @loc_out - location of pagepool page
 * @off_out - offset in page (this is location of entry inside pagepool)
 * @size - mininum required size
 * */
static bool dbms_pa_find(const struct dbms *dbms, const pageoff_t size,
                               fileoff_t *loc_out, pageoff_t *off_out) {
  fileoff_t fileoff = dbms->meta->free_last;
  page_container *pc = NULL;
  bool found = false;
  while (!fileoff_is_null(fileoff)) {
    pc = dbms_pa_select(dbms, fileoff);
    cp_entry_iter *it = cp_entry_iter_construct(pc);
    page_entry *entry = cp_entry_iter_get(it);
    while (entry) {
      if (entry->size.bytes >= size.bytes) {
        found = true;
        // set returned values
        *off_out = it->cur;
        *loc_out = fileoff;
        break;
      }
    }
    cp_entry_iter_destruct(&it);
    fileoff_t prev = pc->header.prev;
    container_destruct(&pc);
    if (found) {
      break;
    } else {
      fileoff = prev;
    }
  }
  return found;
}

// just move file ptr (go through all pages and if )
page_entry dbms_pa_malloc(struct dbms *dbms, const pageoff_t size) {
  FILE *file = dbms->dbfile->file;

  const fileoff_t last_loc = dbms->meta->free_last;
  page_container *cur = dbms_pa_select(dbms, last_loc);
  pageoff_t last_size = cur->header.base.size;

  fileoff_t prev_loc = cur->header.prev;
  // NOTE: insert empty page
  // If current page is empty -> drop it (later add in dbms)
  bool last_is_empty = container_empty(cur);
  // just destruct this page from ram but not delete it currently
  if (last_is_empty) {
    if (!fileoff_is_null(prev_loc)) {
      dbms->meta->free_last = prev_loc;
      container_destruct(&cur);
      cur = dbms_pa_select(dbms, prev_loc);
    }
  }

  fileoff_t cur_loc = last_is_empty ? prev_loc : last_loc;

  fileoff_t sel_loc;
  pageoff_t sel_off;

  page_entry found_entry;
  bool found = dbms_pa_find(dbms, size, &sel_loc, &sel_off);

  if (found) {
    page_container *page;
    if (cur_loc.bytes != sel_loc.bytes) {
      page = dbms_pa_select(dbms, sel_loc);
    } else {
      page = cur;
    }
    // Set to found entry last (last not empty page)
    page_entry *found_ptr = (page_entry *)((void *)page + sel_off.bytes);
    found_entry = *found_ptr;
    *found_ptr = *(page_entry *)((void *)cur + cur->header.start.bytes);
    // pop last element
    container_pop(cur);

    // free page if different
    if (cur_loc.bytes != sel_loc.bytes) {
      container_alter(page, file, sel_loc);
      container_destruct(&page);
    }
  } else {
    found_entry = dbms_pa_alloc(dbms, size);
  }

  // if last is empty and previous is not full -> delete last page
  if (last_is_empty && !container_full(cur)) {
    page_entry entry = (page_entry){.size = last_size, .start = last_loc};
    dbms_pa_free(dbms, &entry);
  } else {// return pointer in meta to normal condition
    dbms->meta->free_last = last_loc;
  }

  container_alter(cur, file, cur_loc);
  container_destruct(&cur);
  return found_entry;
}

void dbms_pa_free(dbms *dbms, const page_entry *entry) {
  fileoff_t last_loc = dbms->meta->dp_last;
  page_container *last = dbms_pa_select(dbms, last_loc);
  // allocate new page for container (in other case it can be bad)
  if (!container_push(last, entry)) {
    // destruct previous last page
    container_destruct(&last);
    // append new page and point to previous one
    dbms_pa_create_append(dbms, last_loc, &last);
    // try again
    assert(container_push(last, entry));
  }
}
