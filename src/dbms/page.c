// Whole handling of pointing dbms and file page storage
#include "page.h"

#include "core/dbfile.h"
#include "core/pagepool.h"
#include "io/meta/meta.h"
#include "io/p_container.h"
#include "io/p_data.h"
#include "io/p_database.h"
#include "io/p_table.h"
#include <assert.h>

const pageoff_t DATABASE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t DATA_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t TABLE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t CONTAINER_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};

static pageoff_t get_page_size(const pageoff_t min_size, const pageoff_t size) {
  return size.bytes > min_size.bytes ? size : min_size;
}

static pageoff_t _tp_page_size(const pageoff_t size, const dp_typle *typle) {
  pageoff_t min_size = get_page_size(TABLE_PAGE_MIN_SIZE, size);
  return get_page_size(min_size, tp_get_min_size(typle));
}

// TODO: insert page in dumped page stack
void dbms_page_drop(dbms *dbms, fileoff_t page_start) {
  page_drop(dbms->dbfile->file, page_start);
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
fileoff_t dbms_dp_create(dbms *dbms, pageoff_t dp_size, database_page **dp_ptr_out) {
  meta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  fileoff_t prev_pos = meta->dp_last;

  database_page *dp = dp_construct_init(get_page_size(DATABASE_PAGE_MIN_SIZE, dp_size),
                                        prev_pos, FILEOFF_NULL);
  fileoff_t dp_pos = meta->pos_empty;
  meta->pos_empty = get_fileoff_t(dp_pos.bytes + dp->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->dp_first) && fileoff_is_null(meta->dp_last)) {
    meta->dp_first = dp_pos;
  } else {
    // There is at least one page allocated
    database_page *prev = dbms_dp_select(dbms, prev_pos);
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
database_page *dbms_dp_select(dbms *dbms, fileoff_t page_start) {
  FILE *file = dbms->dbfile->file;
  pageoff_t size;
  page_load_size(&size, file, page_start);
  database_page *dp = dp_construct(size);
  dp_load(dp, file, page_start);
  return dp;
}

// Alter + destruct
void dbms_dp_close(database_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  dp_alter(*page_ptr, dbms->dbfile->file, page_start);
  dp_destruct(page_ptr);
}

// Create index and
void dbms_dp_insert_table(dbms *dbms, dp_typle *typle) {}

// DATA PAGE
fileoff_t dbms_da_create_close(dbms *dbms, pageoff_t dp_size) {
  data_page *page;
  fileoff_t page_pos = dbms_da_create(dbms, dp_size, &page);
  da_destruct(&page);
  return page_pos;
}

fileoff_t dbms_da_create(dbms *dbms, pageoff_t page_size, data_page **da_ptr_out) {
  dbmeta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  data_page *da = da_construct_init(get_page_size(DATA_PAGE_MIN_SIZE, page_size));
  fileoff_t da_pos = meta->pos_empty;
  meta->pos_empty = get_fileoff_t(da_pos.bytes + da->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->da.first) && fileoff_is_null(meta->da.last)) {
    meta->da.first = da_pos;
  }
  meta->da.last = da_pos;

  da_create(da, file, da_pos);
  *da_ptr_out = da;
  return da_pos;
}

data_page *dbms_da_select(dbms *dbms, fileoff_t page_start) {
  FILE *file = dbms->dbfile->file;
  pageoff_t size;
  page_load_size(&size, file, page_start);
  data_page *da = da_construct(size);
  da_load(da, file, page_start);
  return da;
}

// Alter + destruct
void dbms_da_close(data_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  da_alter(*page_ptr, dbms->dbfile->file, page_start);
  da_destruct(page_ptr);
}

void dbms_da_insert_data(const void *data, size_t size, dbms *dbms,
                         fileoff_t *fileoff_out, pageoff_t *pageoff_out) {

  fileoff_t fileoff = dbms->meta->da.last;
  data_page *da = dbms_da_select(dbms, fileoff);
  pageoff_t pageoff = da_insert_data(da, data, size);

  if (pageoff.bytes == 0) {
    da_destruct(&da);
    // Create new page and insert data in it
    pageoff_t da_size = da_body_page(get_bodyoff_t(size));
    fileoff = dbms_da_create(dbms, da_size, &da);
    pageoff = da_insert_data(da, data, size);
  }
  fileoff_out->bytes = fileoff.bytes;
  pageoff_out->bytes = pageoff.bytes;
  dbms_da_close(&da, *fileoff_out, dbms);
}

// TABLE_PAGE
fileoff_t dbms_tp_create_close(dbms *dbms, pageoff_t size, fileoff_t prev_pos,
                               const dp_typle *typle) {
  table_page *page;
  fileoff_t page_pos = dbms_tp_create(dbms, size, prev_pos, typle, &page);
  tp_destruct(&page);
  return page_pos;
}

fileoff_t dbms_tp_create(dbms *dbms, const pageoff_t size, const fileoff_t prev_pos,
                         const dp_typle *typle, table_page **tp_ptr_out) {
  dbmeta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  pageoff_t page_size = _tp_page_size(size, typle);
  table_page *tp = tp_construct_init(page_size, prev_pos, FILEOFF_NULL, typle);
  fileoff_t tp_pos = meta->pos_empty;

  meta->pos_empty = get_fileoff_t(tp_pos.bytes + tp->header.base.size.bytes);

  if (prev_pos.bytes != 0) {
    table_page *prev = dbms_tp_select(dbms, prev_pos);
    prev->header.next = tp_pos;
    tp->header.prev = prev_pos;
    dbms_tp_close(&prev, prev_pos, dbms);
  }

  tp_create(tp, file, tp_pos);
  *tp_ptr_out = tp;
  return tp_pos;
}

table_page *dbms_tp_select(dbms *dbms, fileoff_t page_start) {
  FILE *file = dbms->dbfile->file;
  pageoff_t size;
  page_load_size(&size, file, page_start);
  table_page *tp = tp_construct(size);
  tp_load(tp, file, page_start);
  return tp;
}

void dbms_tp_close(table_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  tp_alter(*page_ptr, dbms->dbfile->file, page_start);
  tp_destruct(page_ptr);
}

// FREE PAGE CONTAINER
fileoff_t dbms_container_create_close(dbms *dbms, pageoff_t size) {
  page_container *page;
  fileoff_t page_pos = dbms_container_create(dbms, size, &page);
  container_destruct(&page);
  return page_pos;
}
fileoff_t dbms_container_create(dbms *dbms, pageoff_t size,
                                page_container **pc_ptr_out) {
  meta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  fileoff_t prev_pos = meta->free.last;

  page_container *page = container_construct_init(
      get_page_size(CONTAINER_PAGE_MIN_SIZE, size), prev_pos, FILEOFF_NULL);
  fileoff_t page_pos = meta->pos_empty;

  meta->pos_empty = get_fileoff_t(page_pos.bytes + page->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->free.first) && fileoff_is_null(meta->free.last)) {
    meta->free.first = page_pos;
  } else {
    // There is at least one page allocated
    page_container *prev = dbms_container_select(dbms, prev_pos);
    prev->header.next = page_pos;
    page->header.prev = prev_pos;
    dbms_container_close(&prev, prev_pos, dbms);
  }
  meta->free.last = page_pos;

  container_create(page, file, page_pos);
  *pc_ptr_out = page;
  return page_pos;
}

page_container *dbms_container_select(dbms *dbms, fileoff_t page_start) {
  FILE *file = dbms->dbfile->file;
  pageoff_t size;
  page_load_size(&size, file, page_start);
  page_container *tp = container_construct(size);
  container_load(tp, file, page_start);
  return tp;
}
void dbms_container_close(page_container **page_ptr, fileoff_t page_start, dbms *dbms) {
  container_alter(*page_ptr, dbms->dbfile->file, page_start);
  container_destruct(page_ptr);
}
