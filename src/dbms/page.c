// Whole handling of pointing dbms and file page storage
#include "page.h"

#include "core/dbfile.h"
#include "core/meta.h"
#include "core/pagepool.h"

#include "io/data.h"
#include "io/database.h"
#include "io/page/data.h"
#include "io/page/database.h"

const pageoff_t DATABASE_PAGE_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t DATA_PAGE_SIZE = (pageoff_t){.bytes = 1024};

fileoff_t dbms_dp_create(dbms *dbms) {
  dbmeta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  database_page *dp = dp_construct(DATABASE_PAGE_SIZE);
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
    dp_alter(prev, file, prev_pos);
    dp_destruct(&prev);
  }
  meta->dp.last = dp_pos;

  dp_create(dp, file, meta->pos_empty);
  dp_destruct(&dp);
  return dp_pos;
}

fileoff_t dbms_da_create(dbms *dbms) {
  dbmeta *meta = dbms->meta;
  FILE *file = dbms->dbfile->file;

  data_page *da = da_construct(DATA_PAGE_SIZE);
  fileoff_t da_pos = meta->pos_empty;
  meta->pos_empty = get_fileoff_t(da_pos.bytes + da->header.base.size.bytes);
  // If no pages are stored
  if (fileoff_is_null(meta->da.first) && fileoff_is_null(meta->da.last)) {
    meta->da.first = da_pos;
  }
  meta->da.last = da_pos;

  da_create(da, file, meta->pos_empty);
  da_destruct(&da);
  return da_pos;
}
