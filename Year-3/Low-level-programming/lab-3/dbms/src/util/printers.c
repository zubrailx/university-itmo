#include "printers.h"

#include "../dbms/io/page/p_database.h"
#include "../dbms/iter.h"
#include "../dbms/op_schema.h"
#include "../dbms/sso.h"

#include <stdio.h>
#include <stdlib.h>

void print_database_page(struct database_page *page) {
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

void print_database_tuple(struct dp_tuple *typle) {
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

void print_database_tables(struct dbms *dbms) {
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

static void print_table_column_specific(const void *src, const uint8_t col_type) {
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
  default:
    printf("unknown");
  }
}

#undef DECLARE_ASSIGN_VAR

void print_table_tuple(const tp_tuple *tuple, const dp_tuple *dpt,
                       const tpt_col_info *col_info, struct dbms *dbms) {
  printf("\n");
  printf("table_tuple\n");
  printf("is_present: %s\n", tuple->header.is_present ? "true" : "false");
  char **columns = malloc(sizeof(char *) * dpt->header.cols);
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    columns[i] = dbms_sso_construct_select(&dpt->columns[i].sso, dbms);
  }
  for (int i = 0; i < dpt->header.cols; ++i) {
    void *src = (uint8_t *)tuple + col_info[i].start;
    printf("%s: ", columns[i]);
    print_table_column_specific(src, dpt->columns[i].type);
    printf("\n");
  }
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    free(columns[i]);
  }
  free(columns);
}

void print_table_rows(struct dbms *dbms, const char *table_name) {
  fileoff_t dp_loc;
  pageoff_t dp_off;
  bool found = dbms_find_table(table_name, dbms, &dp_loc, &dp_off);
  if (!found)
    return;
  struct database_page *page;
  struct dp_tuple *dpt = dbms_select_tuple(dp_loc, dp_off, dbms, &page);

  const tpt_col_info *info = tp_construct_col_info_arr(dpt);

  printf("----------------------\n");
  printf("table: '%s'\n", table_name);
  tp_iter *iter = tp_iter_construct(dbms, dpt, false);
  tp_tuple *tuple = tp_iter_get(iter);
  while (tuple) {
    print_table_tuple(tuple, dpt, info, dbms);

    tp_iter_next(iter);
    tuple = tp_iter_get(iter);
  }
  tp_iter_destruct(&iter);

  free((void *)info);
  dp_destruct(&page);
  printf("----------------------\n");
}
