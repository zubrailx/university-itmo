// Whole handling of pointing dbms and file page storage
#include "page.h"

#include "core/dbfile.h"
#include "core/pagepool.h"
#include "io/meta/meta.h"
#include "io/p_container.h"
#include "io/p_data.h"
#include "io/p_database.h"
#include "io/p_table.h"
#include "pagealloc.h"
#include <assert.h>

const pageoff_t DATABASE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t DATA_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t TABLE_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};
const pageoff_t CONTAINER_PAGE_MIN_SIZE = (pageoff_t){.bytes = 1024};

static pageoff_t size_max(const pageoff_t min_size, const pageoff_t size) {
  return size.bytes > min_size.bytes ? size : min_size;
}

static pageoff_t _tp_size_max(const pageoff_t size, const dp_typle *typle) {
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
    pc = dbms_container_select(dbms, fileoff);
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

page_container *dbms_container_select(const struct dbms *dbms, const fileoff_t pc_loc) {
  FILE *file = dbms->dbfile->file;
  return container_construct_select(file, pc_loc);
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
  container_alter(*page_ptr, dbms->dbfile->file, page_start);
  container_destruct(page_ptr);
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

  dp_size = size_max(DATABASE_PAGE_MIN_SIZE, dp_size);

  // create page
  page_entry entry = dbms_page_malloc(dbms, dp_size);
  database_page *dp = dp_construct_init(entry.size, prev_pos, FILEOFF_NULL);
  fileoff_t dp_pos = entry.start;

  // If no pages are stored
  if (!fileoff_is_null(meta->dp_last)) {
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
  return dp_construct_select(file, page_start);
}

// Alter + destruct
void dbms_dp_close(database_page **page_ptr, fileoff_t page_start, dbms *dbms) {
  dp_alter(*page_ptr, dbms->dbfile->file, page_start);
  dp_destruct(page_ptr);
}

// DATA PAGE
// fileoff_t dbms_da_create_close(dbms *dbms, pageoff_t dp_size) {
// data_page *page;
// fileoff_t page_pos = dbms_da_create(dbms, dp_size, &page);
// da_destruct(&page);
// return page_pos;
// }

// fileoff_t dbms_da_create(dbms *dbms, pageoff_t page_size, data_page **da_ptr_out) {
//   meta *meta = dbms->meta;
//   FILE *file = dbms->dbfile->file;

//   data_page *da = da_construct_init(get_page_size(DATA_PAGE_MIN_SIZE, page_size));
//   fileoff_t da_pos = meta->pos_empty;
//   meta->pos_empty = get_fileoff_t(da_pos.bytes + da->header.base.size.bytes);
//   // If no pages are stored
//   if (fileoff_is_null(meta->da_first) && fileoff_is_null(meta->da.last)) {
//     meta->da.first = da_pos;
//   }
//   meta->da.last = da_pos;

//   da_create(da, file, da_pos);
//   *da_ptr_out = da;
//   return da_pos;
// }

// data_page *dbms_da_select(dbms *dbms, fileoff_t page_start) {
//   FILE *file = dbms->dbfile->file;
//   pageoff_t size;
//   page_load_size(&size, file, page_start);
//   data_page *da = da_construct(size);
//   da_load(da, file, page_start);
//   return da;
// }

// // Alter + destruct
// void dbms_da_close(data_page **page_ptr, fileoff_t page_start, dbms *dbms) {
//   da_alter(*page_ptr, dbms->dbfile->file, page_start);
//   da_destruct(page_ptr);
// }

// void dbms_da_insert_data(const void *data, size_t size, dbms *dbms,
//                          fileoff_t *fileoff_out, pageoff_t *pageoff_out) {

//   fileoff_t fileoff = dbms->meta->da.last;
//   data_page *da = dbms_da_select(dbms, fileoff);
//   pageoff_t pageoff = da_insert_data(da, data, size);

//   if (pageoff.bytes == 0) {
//     da_destruct(&da);
//     // Create new page and insert data in it
//     pageoff_t da_size = da_body_page(get_bodyoff_t(size));
//     fileoff = dbms_da_create(dbms, da_size, &da);
//     pageoff = da_insert_data(da, data, size);
//   }
//   fileoff_out->bytes = fileoff.bytes;
//   pageoff_out->bytes = pageoff.bytes;
//   dbms_da_close(&da, *fileoff_out, dbms);
// }

// TABLE_PAGE
// fileoff_t dbms_tp_create_close(dbms *dbms, pageoff_t size, fileoff_t prev_pos,
//                                const dp_typle *typle) {
//   table_page *page;
//   fileoff_t page_pos = dbms_tp_create(dbms, size, prev_pos, typle, &page);
//   tp_destruct(&page);
//   return page_pos;
// }

// fileoff_t dbms_tp_create(dbms *dbms, const pageoff_t size, const fileoff_t prev_pos,
//                          const dp_typle *typle, table_page **tp_ptr_out) {
//   meta *meta = dbms->meta;
//   FILE *file = dbms->dbfile->file;

//   pageoff_t page_size = _tp_size_max(size, typle);
//   table_page *tp = tp_construct_init(page_size, prev_pos, FILEOFF_NULL, typle);
//   fileoff_t tp_pos = meta->pos_empty;

//   meta->pos_empty = get_fileoff_t(tp_pos.bytes + tp->header.base.size.bytes);

//   if (prev_pos.bytes != 0) {
//     table_page *prev = dbms_tp_select(dbms, prev_pos);
//     prev->header.next = tp_pos;
//     tp->header.prev = prev_pos;
//     dbms_tp_close(&prev, prev_pos, dbms);
//   }

//   tp_create(tp, file, tp_pos);
//   *tp_ptr_out = tp;
//   return tp_pos;
// }

// table_page *dbms_tp_select(dbms *dbms, fileoff_t page_start) {
//   FILE *file = dbms->dbfile->file;
//   pageoff_t size;
//   page_load_size(&size, file, page_start);
//   table_page *tp = tp_construct(size);
//   tp_load(tp, file, page_start);
//   return tp;
// }

// void dbms_tp_close(table_page **page_ptr, fileoff_t page_start, dbms *dbms) {
//   tp_alter(*page_ptr, dbms->dbfile->file, page_start);
//   tp_destruct(page_ptr);
// }
//
