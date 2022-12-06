#include "op_schema.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "converters/table_tuple.h"
#include "core/dbmeta.h"
#include "page.h"
#include "pagealloc.h"
#include "sso.h"

// create typle and insert sso strings
static struct dp_tuple *create_tuple_and_sso(const struct dto_table *dto_table,
                                             struct dbms *dbms,
                                             size_t *typle_size_out) {
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
    table_column_limits_to_page(&dto_col->lims, &col.limits);
    col.type = table_column_type_to_page(dto_col->type);
    dbms_sso_insert(strlen(dto_col->name) + 1, dto_col->name, dbms);

    tuple->columns[i] = col;
    dto_col = dto_col->next;
  }
  *typle_size_out = tuple_size;
  return tuple;
}

// create pages needed
static void create_tuple_pages(struct dp_tuple *tuple, struct dbms *dbms) {
  tuple->header.tp_first =
      dbms_tp_create_close(dbms, SIZE_DEFAULT, FILEOFF_NULL, tuple);
  tuple->header.tp_last = tuple->header.tp_first;
  // create container and add page inside it
  tuple->header.gappy_last =
      dbms_container_create_close(dbms, FILEOFF_NULL, SIZE_DEFAULT);
}

// Just insert table, no checks for existance
void dbms_create_table(const struct dto_table *dto_table, struct dbms *dbms) {
  size_t typle_size;
  dp_tuple *tuple = create_tuple_and_sso(dto_table, dbms, &typle_size);
  create_tuple_pages(tuple, dbms);

  // select last page and insert there or create new page
  fileoff_t page_loc = dbms->meta->dp_last;
  database_page *page = dbms_dp_open(dbms, page_loc);

  if (dp_insert_table(page, tuple).bytes == 0) {
    dp_destruct(&page);

    pageoff_t size = dp_body_page(get_bodyoff_t(typle_size + sizeof(page_index)));
    page_loc = dbms_dp_create(dbms, size, &page);

    assert(dp_insert_table(page, tuple).bytes > 0);
  }
  // close resources
  dbms_dp_close(&page, page_loc, dbms);
  free(tuple);
}

static void drop_table_pages(const dp_tuple *tuple, struct dbms *dbms) {
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
  // drop gappy pages
  current.start = tuple->header.gappy_last;
  while (!fileoff_is_null(current.start)) {
    page_container *page = dbms_container_open(dbms, current.start);
    current.size = page->header.base.size;

    dbms_page_free(dbms, &current);

    current.start = page->header.prev;
    container_destruct(&page);
  }
}

// return typle inside database_page
static dp_tuple *select_tuple(const fileoff_t fileoff, const pageoff_t idx_pageoff,
                              struct dbms *dbms, database_page **page_out) {
  database_page *page = dbms_dp_open(dbms, fileoff);
  dp_tuple *tuple = dp_tuple_locate(page, idx_pageoff);
  *page_out = page;
  return tuple;
}

bool dbms_drop_table(const fileoff_t fileoff, const pageoff_t idx_pageoff,
                     struct dbms *dbms) {
  database_page *page;
  dp_tuple *tuple = select_tuple(fileoff, idx_pageoff, dbms, &page);

  drop_table_pages(tuple, dbms);
  bool res = dp_drop_table(page, idx_pageoff);

  dbms_dp_close(&page, fileoff, dbms);
  return res;
}
