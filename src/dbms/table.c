#include "io/page/p_database.h"
#include "plan.h"
#include "sso.h"
#include <string.h>

bool dbms_sso_do_update(const page_sso *lsv, const page_sso *rsv) {
  if (lsv->not_inline != rsv->not_inline) {
    return true;
  }

  if (lsv->not_inline && rsv->not_inline) {
    return lsv->po_ptr.fileoff.bytes != rsv->po_ptr.fileoff.bytes ||
           lsv->po_ptr.offset.bytes != rsv->po_ptr.offset.bytes;
  } else {
    return strcmp(lsv->name, rsv->name);
  }
}

void dbms_update_row_pre(const struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
                         const struct dp_tuple *dpt,
                         const struct tpt_col_info *col_info, struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    // If current column is string and match do_update case then remove sso string
    if (dpt->columns[i].type == COLUMN_TYPE_STRING) {
      size_t off = col_info[i].start;
      if (dbms_sso_do_update((void *)tpt_old + off, (void *)tpt_new + off)) {

        struct TPT_COL_TYPE(COLUMN_TYPE_STRING) *old_str_col = (void *)tpt_old + off;
        dbms_sso_remove(&old_str_col->entry, dbms);
      }
    }
  }
}

void dbms_remove_row_pre(const struct tp_tuple *old, const struct dp_tuple *dpt,
                         const struct tpt_col_info *col_info, struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    if (dpt->columns[i].type == COLUMN_TYPE_STRING) {
      size_t off = col_info[i].start;
      struct TPT_COL_TYPE(COLUMN_TYPE_STRING) *old_str_col = (void *)old + off;
      dbms_sso_remove(&old_str_col->entry, dbms);
    }
  }
}
