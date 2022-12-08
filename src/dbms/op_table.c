#include "op_table.h"
#include "core/dbmeta.h"
#include "core/dbms.h"
#include "dto/dto_row.h"
#include "io/page/p_database.h"
#include "page.h"
#include "table_dist.h"

static fileoff_t get_table_page(dp_tuple *tuple, const struct dto_row *row,
                                struct dbms *dbms, table_page **page_out) {
  page_entry entry;
  // fix aligned warning
  fileoff_t aligned = tuple->header.td_last;
  bool is_malloc = !dbms_td_pop_single(dbms, tuple, &aligned, &entry);
  tuple->header.td_last = aligned;

  if (is_malloc) {
    *page_out =
        tp_construct_init(entry.size, tuple->header.tp_last, FILEOFF_NULL, tuple);
  } else {
    *page_out = dbms_tp_open(dbms, entry.start);
  }
  return entry.start;
}

int dbms_insert_row(dp_tuple *tuple, const struct dto_row *row, struct dbms *dbms) {
  table_page *page;

  fileoff_t page_loc = get_table_page(tuple, row, dbms, &page);
  // tp_
}
