#include "../io/page/p_database.h"
#include "../io/page/p_table.h"
#include "../sso.h"
#include <dbms/tp_tuple.h>
#include <stdio.h>
#include <string.h>

const char *tp_column_to_str(const struct tpt_column_base *tpt_base,
                             const struct dpt_column *dpt_column, struct dbms *dbms) {
  if (dpt_column->limits.is_null) {
    return strdup("");
  }

  switch (dpt_column->type) {

  case COLUMN_TYPE_BOOL: {
    struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *column = (void *)tpt_base;
    return column->entry ? strdup("true") : strdup("false");
  }

  case COLUMN_TYPE_STRING: {
    struct TPT_COL_TYPE(COLUMN_TYPE_STRING) *column = (void *)tpt_base;
    return dbms_sso_construct_select(&column->entry, dbms);
  }

  case COLUMN_TYPE_INT32: {
    char ibuf[16];
    struct TPT_COL_TYPE(COLUMN_TYPE_INT32) *column = (void *)tpt_base;
    sprintf(ibuf, "%d", column->entry);
    return strdup(ibuf);
  }

  case COLUMN_TYPE_DOUBLE: {
    char ibuf[16];
    struct TPT_COL_TYPE(COLUMN_TYPE_DOUBLE) *column = (void *)tpt_base;
    snprintf(ibuf, 16, "%f", column->entry);
    return strdup(ibuf);
  }

  default:
    return NULL;
  }
}
