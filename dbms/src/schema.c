#include <dbms/schema.h>
#include <string.h>

#include "dbms/dto_table.h"
#include "dbms/op_schema.h"
#include "dbms/page.h"
#include "dbms/converters/table.h"
#include "dbms/sso.h"

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
    struct dto_table *table = dto_table_construct(table_name);
    table->name = strdup(table_name);

    struct database_page *dp;
    struct dp_tuple *tuple = dbms_select_tuple(fileoff, pageoff, dbms, &dp);

    for (size_t i = 0; i < tuple->header.cols; ++i) {
      dpt_column column = tuple->columns[i];
      const char *col_name = dbms_sso_construct_select(&column.sso, dbms);
      dto_table_column_limits lims;
      column_limits_to_dto(&column.limits, &lims);
      dto_table_add_column(table, col_name, column.type, lims);
    }

    dp_destruct(&dp);
    return table;
  }
}
