#include "pagealloc.h"

#include "core/dbfile.h"
#include "core/dbms.h"
#include "io/meta/meta.h"
#include "io/p_container.h"
#include "page.h"

const pageoff_t PAGEPOOL_PAGE_SIZE = (pageoff_t){.bytes = 1024};

// create last page for pagepool meta->free_last is managed automatically
// @prev - previous page
static fileoff_t container_create_append(struct dbms *dbms, const fileoff_t prev,
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
  container_create_append(dbms, prev, &pc);
  container_destruct(&pc);
}
// another implementation not write to file
static page_entry alloc_page_entry(struct dbms *dbms, pageoff_t size) {
  base_page *page = page_construct(size, PAGE_UNKNOWN);
  page_create(page, dbms->dbfile->file, dbms->meta->pos_empty);
  page_entry entry =
      (page_entry){.size = page->header.size, .start = dbms->meta->pos_empty};

  dbms->meta->pos_empty.bytes += page->header.size.bytes;
  page_destruct(&page);
  return entry;
}

// just move file ptr (go through all pages and if )
page_entry dbms_page_malloc(struct dbms *dbms, const pageoff_t size) {
  FILE *file = dbms->dbfile->file;

  const fileoff_t last_loc = dbms->meta->free_last;
  page_container *cur = dbms_container_open(dbms, last_loc);
  pageoff_t last_size = cur->header.base.size;

  fileoff_t prev_loc = cur->header.prev;

  bool remove_last = container_is_empty(cur) && !fileoff_is_null(prev_loc);
  // Destruct this page from ram but not delete it currently
  if (remove_last) {
    dbms->meta->free_last = prev_loc;
    container_destruct(&cur);
    cur = dbms_container_open(dbms, prev_loc);
  }

  fileoff_t cur_loc = remove_last ? prev_loc : last_loc;

  fileoff_t sel_loc;
  pageoff_t sel_off;

  page_entry found_entry;
  bool found = dbms_container_find(dbms, size, &sel_loc, &sel_off);

  if (found) {
    page_container *page;
    if (cur_loc.bytes != sel_loc.bytes) {
      page = dbms_container_open(dbms, sel_loc);
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
    found_entry = alloc_page_entry(dbms, size);
  }

  // if last is empty and previous is not full -> delete last page
  // if found -> cur page is not full because of container_pop
  if (remove_last && found) {
    page_entry entry = (page_entry){.size = last_size, .start = last_loc};
    dbms_page_free(dbms, &entry);
  } else {// return pointer in meta to normal condition
    dbms->meta->free_last = last_loc;
  }

  container_alter(cur, file, cur_loc);
  container_destruct(&cur);
  return found_entry;
}

void dbms_page_free(dbms *dbms, const page_entry *entry) {
  fileoff_t last_loc = dbms->meta->free_last;
  page_container *last = dbms_container_open(dbms, last_loc);
  // allocate new page for container (in other case it can be bad)
  if (container_is_full(last)) {
    // destruct previous last page
    container_destruct(&last);
    // append new page and point to previous one
    last_loc = container_create_append(dbms, last_loc, &last);
  }
  container_push(last, entry);
  container_alter(last, dbms->dbfile->file, last_loc);
  container_destruct(&last);
}
