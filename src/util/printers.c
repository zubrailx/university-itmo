#include "printers.h"
#include <stdlib.h>

void print_database_page(database_page *page) {
  printf("----------------------\n");
  printf("database_page\n");
  printf("size: %u\n", page->header.base.size.bytes);
  printf("type: %hhu\n", page->header.base.type);
  printf("index_start: %u\n", page->header.index_start.bytes);
  printf("typle_end: %u\n", page->header.tuple_end.bytes);
  printf("next: %zu\n", page->header.next.bytes);
  printf("prev: %zu\n", page->header.prev.bytes);
  printf("----------------------\n");
}

void print_database_tuple(dp_tuple *typle) {
  printf("----------------------\n");
  printf("database_tuple\n");
  printf("cols: %zu\n", typle->header.cols);
  printf("is_present: %u\n", typle->header.is_present);
  printf("first_page: %zu\n", typle->header.tp_first.bytes);
  printf("last_page: %zu\n", typle->header.tp_last.bytes);
  printf("gappy_page: %zu\n", typle->header.td_last.bytes);
  printf("not_inline: %d\n", (int)typle->header.sso.not_inline);
  printf("----------------------\n");
}

void print_database_tables(dbms *dbms) {
  dp_iter *iter = dp_iter_construct(dbms);
  dp_tuple *tuple = dp_iter_get(iter);
  while (tuple) {
    print_database_tuple(tuple);

    dp_iter_next(iter);
    tuple = dp_iter_get(iter);
  }
  dp_iter_destruct(&iter);
}

#define DECLARE_ASSIGN_VAR(var_name, assigned, type) type var_name = (type)assigned

static void print_table_tuple_specific(const void *src, const uint8_t col_type) {
  printf("entry: ");

  switch (col_type) {
  case COLUMN_TYPE_BOOL: {
    const DECLARE_ASSIGN_VAR(col, src, struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *);
    printf("%s", col->entry ? "true" : "false");
    break;
  }
  case COLUMN_TYPE_DOUBLE: {
    const DECLARE_ASSIGN_VAR(col, src, struct TPT_COL_TYPE(COLUMN_TYPE_DOUBLE) *);
    printf("%f", col->entry);
    break;
  }
  case COLUMN_TYPE_INT32: {
    const DECLARE_ASSIGN_VAR(col, src, struct TPT_COL_TYPE(COLUMN_TYPE_INT32) *);
    printf("%d", col->entry);
    break;
  }
  case COLUMN_TYPE_STRING: {
    const DECLARE_ASSIGN_VAR(col, src, struct TPT_COL_TYPE(COLUMN_TYPE_STRING) *);
    if (col->entry.not_inline) {
      printf("[non_inlined], po_ptr(%zu, %u), size(%zu)",
             col->entry.po_ptr.fileoff.bytes, col->entry.po_ptr.offset.bytes,
             sso_to_size(col->entry.ssize));
    } else {
      printf("[inlined] '%s'", col->entry.name);
    }
    break;
  }
  }
  printf("\n");
}

#undef DECLARE_ASSIGN_VAR

static void print_table_tuple(const tp_tuple *tuple, const dp_tuple *dpt,
                              const tpt_col_info *col_info) {
  printf("----------------------\n");
  printf("table_tuple\n");
  printf("is_present: %s\n", tuple->header.is_present ? "true" : "false");
  for (int i = 0; i < dpt->header.cols; ++i) {
    void *src = (uint8_t *)tuple + col_info[i].start;
    print_table_tuple_specific(src, dpt->columns[i].type);
  }
  printf("----------------------\n");
}

void print_table_rows(dbms *dbms, const char *table_name) {
  fileoff_t dp_loc;
  pageoff_t dp_off;
  bool found = dbms_find_table(table_name, dbms, &dp_loc, &dp_off);
  if (!found)
    return;
  struct database_page *page;
  struct dp_tuple *dpt = dbms_select_tuple(dp_loc, dp_off, dbms, &page);

  const tpt_col_info *info = tp_constuct_col_info_arr(dpt);

  tp_iter *iter = tp_iter_construct(dbms, dpt);
  tp_tuple *tuple = tp_iter_get(iter);
  while (tuple) {
    print_table_tuple(tuple, dpt, info);

    tp_iter_next(iter);
    tuple = tp_iter_get(iter);
  }
  tp_iter_destruct(&iter);

  free((void *)info);
  dp_destruct(&page);
}
