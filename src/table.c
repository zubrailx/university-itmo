#include "table.h"

#include "dbms/op_sso.h"
#include "dbms/op_table.h"
#include "dbms/page.h"
#include "dbms/tuple_iter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return position of index
bool table_find(struct dbms *dbms, struct dto_table *table, fileoff_t *fileoff_out,
                pageoff_t *pageoff_out) {
  dp_iter *iter = dp_iter_construct(dbms);
  dp_tuple *tuple = dp_iter_get(iter);

  bool found = false;
  while (!found && tuple) {
    char *name = dbms_sso_construct_select(&tuple->header.sso, dbms);
    if (!strcmp(name, table->name)) {
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

bool table_exists(struct dbms *dbms, struct dto_table *table) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  return table_find(dbms, table, &fileoff, &pageoff);
}

bool table_create(struct dbms *dbms, struct dto_table *table) {
  if (table_exists(dbms, table)) {
    return false;
  } else {
    dbms_create_table(table, dbms);
    return true;
  }
}

bool table_drop(struct dbms *dbms, struct dto_table *table) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  if (table_find(dbms, table, &fileoff, &pageoff)) {
    return dbms_drop_table(fileoff, pageoff, dbms);
  }
  return false;
}
