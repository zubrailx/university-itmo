#include <dbms/schema.h>
#include <string.h>

#include "dbms/converters/table.h"
#include "dbms/internals/dto_tuple.h"
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

bool table_drop(struct dbms *dbms, const char *table_name) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  if (dbms_find_table(table_name, dbms, &fileoff, &pageoff)) {
    return dbms_drop_table(fileoff, pageoff, dbms);
  }
  return false;
}

struct dto_table *table_construct_header(struct dbms *dbms, const char *table_name) {
  fileoff_t fileoff;
  pageoff_t pageoff;
  if (!dbms_find_table(table_name, dbms, &fileoff, &pageoff)) {
    return NULL;
  } else {
    struct database_page *dp;
    struct dp_tuple *tuple = dbms_select_tuple(fileoff, pageoff, dbms, &dp);

    struct dto_table *dto_table;
    dp_tuple_to_dto(tuple, &dto_table, dbms);

    dp_destruct(&dp);
    return dto_table;
  }
}
