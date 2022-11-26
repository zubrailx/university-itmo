// Whole handling of pointing dbms and file page storage
#include "page.h"

#include "core/dbfile.h"
#include "core/meta.h"
#include "core/pagepool.h"

#include "io/data.h"
#include "io/database.h"
#include "io/page/data.h"
#include "io/page/database.h"
#include <assert.h>

const pageoff_t DATABASE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t DATA_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};

static pageoff_t get_page_size(const pageoff_t min_size, const pageoff_t size) {
  return size.bytes > min_size.bytes ? size : min_size;
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
  dbmeta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  database_page *dp = dp_construct(get_page_size(DATABASE_PAGE_MIN_SIZE, dp_size));
  fileoff_t dp_pos = meta->pos_empty;
  meta->pos_empty = get_fileoff_t(dp_pos.bytes + dp->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->dp.first) && fileoff_is_null(meta->dp.last)) {
    meta->dp.first = dp_pos;
  } else {
    // There is at least one page allocated
    fileoff_t prev_pos = meta->dp.last;
    pageoff_t prev_size;
    page_load_size(&prev_size, file, prev_pos);
    database_page *prev = dp_construct(prev_size);
    dp_load(prev, file, prev_pos);
    // Link previous page with current
    prev->header.next = dp_pos;
    dp->header.prev = prev_pos;
    dbms_dp_close(&prev, prev_pos, dbms);
  }
  meta->dp.last = dp_pos;

  dp_create(dp, file, meta->pos_empty);
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

  data_page *da = da_construct(get_page_size(DATA_PAGE_MIN_SIZE, page_size));
  fileoff_t da_pos = meta->pos_empty;
  meta->pos_empty = get_fileoff_t(da_pos.bytes + da->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->da.first) && fileoff_is_null(meta->da.last)) {
    meta->da.first = da_pos;
  }
  meta->da.last = da_pos;

  da_create(da, file, meta->pos_empty);
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
    pageoff_t da_size = da_bodyoff_to_pageoff(get_bodyoff_t(size));
    fileoff = dbms_da_create(dbms, da_size, &da);
    pageoff = da_insert_data(da, data, size);
  }
  fileoff_out->bytes = fileoff.bytes;
  pageoff_out->bytes = pageoff.bytes;
  dbms_da_close(&da, *fileoff_out, dbms);
}
