#include "table.h"

#include "dbms/iterator.h"
#include "dbms/operations.h"
#include "dbms/page.h"
#include "dbms/sso.h"
#include <string.h>

bool dbms_table_exists(struct dbms *dbms, struct dto_table *table) {
  dp_iter *iter = dp_iter_construct(dbms);
  dp_typle *typle = dp_iter_get(iter);
  while (typle) {
    char *name;
    struct data_page *da = dbms_get_sso_string(&name, &typle->header.sso, dbms);
    if (!strcmp(name, table->name)) {
      dp_iter_destruct(&iter);
      return true;
    }
    da_destruct(&da);

    dp_iter_next(iter, dbms);
    typle = dp_iter_get(iter);
  }
  dp_iter_destruct(&iter);
  return false;
}

bool dbms_create_table(struct dbms *dbms, struct dto_table *table) {
  if (dbms_table_exists(dbms, table)) {
    return false;
  } else {
    dbms_insert_table(table, dbms);
    return true;
  }
}

bool dbms_drop_table(struct dto_table *table) {}
