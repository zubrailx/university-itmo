// Whole handling of pointing dbms and file page storage
#include "page.h"

#include "core/dbfile.h"
#include "io/meta/meta.h"
#include "io/p_container.h"
#include "io/p_database.h"
#include "io/p_table.h"
#include "pagealloc.h"

const pageoff_t DATABASE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t TABLE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t CONTAINER_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};

static pageoff_t size_max(const pageoff_t min_size, const pageoff_t size) {
  return size.bytes > min_size.bytes ? size : min_size;
}

static pageoff_t tp_size_max(const pageoff_t size, const dp_tuple *typle) {
  pageoff_t min_size = size_max(TABLE_PAGE_MIN_SIZE, size);
  return size_max(min_size, tp_get_min_size(typle));
}

// PAGE CONTAINER
/*
 * @loc_out - location of pagepool page
 * @off_out - offset in page (this is location of entry inside pagepool)
 * @size - mininum required size
 * */
bool dbms_container_find(const struct dbms *dbms, const pageoff_t size,
                         fileoff_t *loc_out, pageoff_t *off_out) {
  fileoff_t fileoff = dbms->meta->free_last;
  page_container *pc = NULL;
  bool found = false;
  while (!fileoff_is_null(fileoff)) {
    pc = dbms_container_open(dbms, fileoff);
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

page_container *dbms_container_open(const struct dbms *dbms, const fileoff_t pc_loc) {
  return container_construct_select(dbms->dbfile->file, pc_loc);
}

// uses pagealloc
fileoff_t dbms_container_create_close(dbms *dbms, const fileoff_t prev,
                                      pageoff_t size) {
  page_container *page;
  fileoff_t page_pos = dbms_container_create(dbms, size, prev, &page);
  container_destruct(&page);
  return page_pos;
}

// uses pagealloc
fileoff_t dbms_container_create(dbms *dbms, pageoff_t size, const fileoff_t prev,
                                page_container **pc_ptr_out) {
  FILE *file = dbms->dbfile->file;

  size = size_max(CONTAINER_PAGE_MIN_SIZE, size);

  page_entry entry = dbms_page_malloc(dbms, size);
  page_container *cont = container_construct_init(entry.size, prev);
  fileoff_t page_pos = entry.start;

  container_create(cont, file, page_pos);
  *pc_ptr_out = cont;
  return page_pos;
}

void dbms_container_close(page_container **page_ptr, fileoff_t page_start, dbms *dbms) {
  container_alter_destruct(page_ptr, dbms->dbfile->file, page_start);
}

// DATABASE PAGE
// Create page and close it immediately
fileoff_t dbms_dp_create_close(dbms *dbms, pageoff_t dp_size) {
  database_page *page;
  fileoff_t page_pos = dbms_dp_create(dbms, dp_size, &page);
  dp_destruct(&page);
  return page_pos;
}

// Create page and return pointer to created page
fileoff_t dbms_dp_create(dbms *dbms, const pageoff_t dp_size,
                         database_page **dp_ptr_out) {
  meta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  fileoff_t prev_pos = meta->dp_last;

  // create page
  page_entry entry = dbms_page_malloc(dbms, size_max(DATABASE_PAGE_MIN_SIZE, dp_size));
  database_page *dp = dp_construct_init(entry.size, prev_pos, FILEOFF_NULL);
  fileoff_t dp_pos = entry.start;

  // If no pages are stored
  if (!fileoff_is_null(meta->dp_last)) {
    // There is at least one page allocated
    database_page *prev = dbms_dp_open(dbms, prev_pos);
    prev->header.next = dp_pos;
    dp->header.prev = prev_pos;
    dbms_dp_close(&prev, prev_pos, dbms);
  }
  meta->dp_last = dp_pos;

  dp_create(dp, file, dp_pos);
  *dp_ptr_out = dp;
  return dp_pos;
}

// Construct + load
database_page *dbms_dp_open(dbms *dbms, fileoff_t page_start) {
  return dp_construct_select(dbms->dbfile->file, page_start);
}

// Alter + destruct
void dbms_dp_close(database_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  dp_alter_destruct(page_ptr, dbms->dbfile->file, page_start);
}

// TABLE_PAGE
fileoff_t dbms_tp_create_close(dbms *dbms, pageoff_t size, fileoff_t prev_pos,
                               const dp_tuple *tuple) {
  table_page *page;
  fileoff_t page_pos = dbms_tp_create(dbms, size, prev_pos, tuple, &page);
  tp_destruct(&page);
  return page_pos;
}

fileoff_t dbms_tp_create(dbms *dbms, const pageoff_t size, const fileoff_t prev_pos,
                         const dp_tuple *tuple, table_page **tp_ptr_out) {
  meta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  // create page
  page_entry entry = dbms_page_malloc(dbms, tp_size_max(size, tuple));
  table_page *tp = tp_construct_init(entry.size, prev_pos, FILEOFF_NULL, tuple);
  fileoff_t tp_pos = meta->pos_empty;

  if (prev_pos.bytes != 0) {
    table_page *prev = dbms_tp_open(dbms, prev_pos);
    prev->header.next = tp_pos;
    tp->header.prev = prev_pos;
    dbms_tp_close(&prev, prev_pos, dbms);
  }

  tp_create(tp, file, tp_pos);
  *tp_ptr_out = tp;
  return tp_pos;
}

table_page *dbms_tp_open(dbms *dbms, fileoff_t page_start) {
  return tp_construct_select(dbms->dbfile->file, page_start);
}

void dbms_tp_close(table_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  tp_alter_destruct(page_ptr, dbms->dbfile->file, page_start);
}
