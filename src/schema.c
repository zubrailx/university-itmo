#include <schema.h>

#include "dbms/op_schema.h"
#include "dbms/page.h"

bool table_exists(struct dbms *dbms, const char *table_name) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  return dbms_find_table(table_name, dbms, &fileoff, &pageoff);
}

bool table_create(struct dbms *dbms, struct dto_table *table) {
  if (table_exists(dbms, table->name)) {
    return false;
  } else {
    dbms_create_table(table, dbms);
    return true;
  }
}

bool table_drop(struct dbms *dbms, struct dto_table *table) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  if (dbms_find_table(table->name, dbms, &fileoff, &pageoff)) {
    return dbms_drop_table(fileoff, pageoff, dbms);
  }
  return false;
}
