#include "table.h"

#include <string.h>

#include "dbms/core/meta.h"
#include "dbms/operations.h"
#include "dbms/dto/table.h"
#include "dbms/io/page/data.h"
#include "dbms/io/page/database.h"
#include "dbms/iterator.h"
#include "dbms/page.h"
#include "dbms/sso.h"

bool dbms_table_exists(struct dbms *dbms, struct dto_table *table) {
  dp_iter *iter = dp_iter_construct(dbms);
  dp_typle *typle = dp_iter_get(iter);
  while (typle) {
    char *name;
    struct data_page *da = dbms_get_sso_string(&name, &typle->header.sso, dbms);
    if (!strcmp(name, table->name)) {
      return true;
    }
    da_destruct(&da);
  }
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
