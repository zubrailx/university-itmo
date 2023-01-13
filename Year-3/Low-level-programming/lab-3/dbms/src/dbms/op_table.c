#include "op_table.h"
#include "core/dbmeta.h"
#include "core/dbms.h"
#include "dto/dto_row.h"
#include "io/page/p_database.h"
#include "page.h"
#include "sso.h"
#include "table.h"
#include "table_dist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static fileoff_t td_page_open(dp_tuple *tuple, struct dbms *dbms,
                              table_page **page_out) {
  page_entry entry;
  // fix aligned warning
  fileoff_t aligned = tuple->header.td_last;
  bool is_malloc = !dbms_td_pop_single(dbms, tuple, &aligned, &entry);
  tuple->header.td_last = aligned;

  if (is_malloc) {
    *page_out =
        tp_construct_init(entry.size, tuple->header.tp_last, FILEOFF_NULL, tuple);

    if (fileoff_is_null(tuple->header.tp_first)) {
      tuple->header.tp_first = entry.start;
    } else {
      // update linked list
      table_page *prev = dbms_tp_open(dbms, tuple->header.tp_last);
      prev->header.next = entry.start;
      dbms_tp_close(&prev, tuple->header.tp_last, dbms);
    }
    // update tp_last
    tuple->header.tp_last = entry.start;
  } else {
    *page_out = dbms_tp_open(dbms, entry.start);
  }
  return entry.start;
}

static void td_page_close(table_page **page_ptr, fileoff_t page_loc, dp_tuple *tuple,
                          struct dbms *dbms) {
  if (!tp_is_full(*page_ptr)) {
    page_entry entry = {.start = page_loc, .size = (*page_ptr)->header.base.size};
    // aligned
    fileoff_t td_last = tuple->header.td_last;
    dbms_td_push_single(dbms, &td_last, &entry);
    tuple->header.td_last = td_last;
  }// maybe later add when page is free?
  dbms_tp_close(page_ptr, page_loc, dbms);
}

static void to_tuple_with_sso(tp_tuple *dest, const void **src, const dp_tuple *dpt,
                              tpt_col_info col_info_arr[], struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    tpt_column_base *colh = (void *)dest + col_info_arr[i].start;
    // General
    {
      colh->is_null = src[i] == NULL;
      if (colh->is_null)
        continue;
    }
    // Specific
    tpt_memcpy_specific(colh, src[i], dpt->columns[i].type, dbms);
  }
}

int dbms_insert_row_list(dp_tuple *dpt, const struct dto_row_list *list,
                         struct dbms *dbms) {
  table_page *page;
  fileoff_t page_loc = td_page_open(dpt, dbms, &page);
  size_t tpt_size = tp_get_tuple_size(dpt);

  tpt_col_info *tpt_info_arr = tp_construct_col_info_arr(dpt);

  // Insert rows
  tp_tuple *tpt = calloc(tpt_size, 1);
  int cnt_inserted = 0;

  for (struct dto_row_node *cur = list->first; cur != NULL;
       cur = dto_row_node_next(cur)) {

    if (tp_is_full(page)) {
      td_page_close(&page, page_loc, dpt, dbms);
      page_loc = td_page_open(dpt, dbms, &page);
    }

    to_tuple_with_sso(tpt, dto_row_node_get(cur), dpt, tpt_info_arr, dbms);
    // set is_present flag and insert
    tpt->header.is_present = true;
    pageoff_t off = tp_insert_row(page, tpt, tp_get_tuple_size(dpt));
    if (off.bytes != 0) {
      ++cnt_inserted;
    }
  }

  free(tpt);
  free(tpt_info_arr);
  td_page_close(&page, page_loc, dpt, dbms);

  return cnt_inserted;
}

int dbms_find_column_idx(const dp_tuple *dpt, const char *column_name,
                         struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    char *name = dbms_sso_construct_select(&dpt->columns[i].sso, dbms);
    if (strcmp(column_name, name) == 0) {
      free(name);
      return i;
    }
    free(name);
  }
  return -1;
}
