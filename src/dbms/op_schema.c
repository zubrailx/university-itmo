#include "op_schema.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "converters/table.h"
#include "core/dbmeta.h"
#include "iter.h"
#include "page.h"
#include "pagealloc.h"
#include "sso.h"
#include "table_dist.h"

// create typle and insert sso strings
static struct dp_tuple *to_tuple_with_sso(const struct dto_table *dto_table,
                                          struct dbms *dbms, size_t *typle_size_out) {
  // Convert dto to entity
  dpt_header header = {};
  {
    header.cols = dto_table_columns(dto_table);
    header.sso = dbms_sso_insert(strlen(dto_table->name) + 1, dto_table->name, dbms);
    header.is_present = true;
    header.tp_first = FILEOFF_NULL;
    header.tp_last = FILEOFF_NULL;
  }

  // construct dptyple
  size_t tuple_size = dp_tuple_size(header.cols);
  dp_tuple *tuple = calloc(tuple_size, 1);
  tuple->header = header;

  dto_table_column *dto_col = dto_table->first;
  dpt_column col = {};
  for (size_t i = 0; i < tuple->header.cols; ++i) {
    // convert dto column to entity
    column_limits_to_page(&dto_col->lims, &col.limits);
    col.type = column_type_to_page(dto_col->type);
    dbms_sso_insert(strlen(dto_col->name) + 1, dto_col->name, dbms);

    tuple->columns[i] = col;
    dto_col = dto_col->next;
  }
  *typle_size_out = tuple_size;
  return tuple;
}

// Just insert table, no checks for existance
void dbms_create_table(const struct dto_table *dto_table, struct dbms *dbms) {
  size_t typle_size;

  dp_tuple *tuple = to_tuple_with_sso(dto_table, dbms, &typle_size);
  tuple->header.td_last = dbms_td_create_close(dbms);

  // select last page and insert there or create new page
  fileoff_t page_loc = dbms->meta->dp_last;
  database_page *page = dbms_dp_open(dbms, page_loc);

  if (dp_insert_table(page, tuple).bytes == 0) {
    dp_destruct(&page);

    pageoff_t size = dp_body_page(get_bodyoff_t(typle_size + sizeof(page_index)));
    page_loc = dbms_dp_create(dbms, size, &page);

    assert(dp_insert_table(page, tuple).bytes != 0);
  }
  // close resources
  dbms_dp_close(&page, page_loc, dbms);
  free(tuple);
}

static void drop_table_pages(dp_tuple *tuple, struct dbms *dbms) {
  page_entry current;
  // drop table pages
  current.start = tuple->header.tp_first;
  while (!fileoff_is_null(current.start)) {
    table_page *page = dbms_tp_open(dbms, current.start);
    current.size = page->header.base.size;

    dbms_page_free(dbms, &current);

    current.start = page->header.next;
    tp_destruct(&page);
  }
  tuple->header.tp_first = FILEOFF_NULL;
  tuple->header.tp_last = FILEOFF_NULL;

  // drop gappy pages
  dbms_td_drop(dbms, tuple->header.td_last);
  tuple->header.td_last = FILEOFF_NULL;
}

// return typle inside database_page
dp_tuple *dbms_select_tuple(const fileoff_t fileoff, const pageoff_t idx_pageoff,
                                   struct dbms *dbms, database_page **page_out) {
  database_page *page = dbms_dp_open(dbms, fileoff);
  dp_tuple *tuple = dp_tuple_locate(page, idx_pageoff);
  *page_out = page;
  return tuple;
}

bool dbms_drop_table(const fileoff_t fileoff, const pageoff_t idx_pageoff,
                     struct dbms *dbms) {
  database_page *page;
  dp_tuple *tuple = dbms_select_tuple(fileoff, idx_pageoff, dbms, &page);

  drop_table_pages(tuple, dbms);

  bool res = dp_drop_table(page, idx_pageoff);
  tuple->header.is_present = false;

  dbms_dp_close(&page, fileoff, dbms);
  return res;
}

// returns location of table in file
bool dbms_find_table(const char *table_name, struct dbms *dbms, fileoff_t *fileoff_out,
                     pageoff_t *pageoff_out) {
  dp_iter *iter = dp_iter_construct(dbms);
  dp_tuple *tuple = dp_iter_get(iter);

  bool found = false;
  while (!found && tuple) {
    char *name = dbms_sso_construct_select(&tuple->header.sso, dbms);
    if (!strcmp(name, table_name)) {
      *fileoff_out = dp_iter_cur_page(iter);
      *pageoff_out = dp_iter_cur_index(iter);
      found = true;
    }
    // free data
    free(name);
    // iterate next
    dp_iter_next(iter);
    tuple = dp_iter_get(iter);
  }
  dp_iter_destruct(&iter);
  return found;
}
