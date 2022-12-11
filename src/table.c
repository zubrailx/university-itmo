#include "table.h"
#include "dbms/op_schema.h"
#include "dbms/op_table.h"
#include "dbms/page.h"

int row_list_insert(struct dbms *dbms, const char *table_name,
                    struct dto_row_list *list) {
  fileoff_t dp_loc;
  pageoff_t dp_off;
  bool found = dbms_find_table(table_name, dbms, &dp_loc, &dp_off);
  if (!found) {
    return 0;
  }
  struct database_page *page;
  struct dp_tuple *dpt = dbms_select_tuple(dp_loc, dp_off, dbms, &page);

  int result = dbms_insert_row_list(dpt, list, dbms);

  dbms_dp_close(&page, dp_loc, dbms);
  return result;
}
