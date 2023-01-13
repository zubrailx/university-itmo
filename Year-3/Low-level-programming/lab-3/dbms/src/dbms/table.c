#include "table.h"
#include "io/page/p_database.h"
#include "io/page/p_table.h"
#include "sso.h"
#include <assert.h>
#include <string.h>

// @dest - aka tpt_column_ENUM_TYPE (depends on col_type)
void tpt_memcpy_specific(void *dest, const void *src, const uint8_t col_type,
                         struct dbms *dbms) {
  switch (col_type) {
  case COLUMN_TYPE_BOOL: {
    struct TPT_COL_TYPE(COLUMN_TYPE_BOOL) *col = dest;
    col->entry = *(bool *)src;
    break;
  }
  case COLUMN_TYPE_DOUBLE: {
    struct TPT_COL_TYPE(COLUMN_TYPE_DOUBLE) *col = dest;
    col->entry = *(double *)src;
    break;
  }
  case COLUMN_TYPE_INT32: {
    struct TPT_COL_TYPE(COLUMN_TYPE_INT32) *col = dest;
    col->entry = *(int32_t *)src;
    break;
  }
  case COLUMN_TYPE_STRING: {
    struct TPT_COL_TYPE(COLUMN_TYPE_STRING) *col = dest;
    col->entry = dbms_sso_insert(strlen(src) + 1, src, dbms);
    break;
  }
  default:
    assert(0 && "ERROR: unknown column type in entry inside p_database.\n");
  }
}

static bool type_can_not_inline(uint8_t type) { return type == COLUMN_TYPE_STRING; }

static bool tpt_should_erase_column(const tpt_column_base *old_base,
                                    const tpt_column_base *new_base,
                                    const uint8_t type) {
  if (!type_can_not_inline(type) || old_base == NULL) {
    return false;
  }
  if (new_base == NULL) {
    return true;
  }

  switch (type) {
  case COLUMN_TYPE_STRING: {
    struct TPT_COL_TYPE(COLUMN_TYPE_STRING) * cold, *cnew;
    cold = (void *)old_base;
    cnew = (void *)new_base;
    // if stored differently
    if (cold->entry.not_inline != cnew->entry.not_inline) {
      return true;
    }
    // if are not equals
    if (cold->entry.not_inline && cnew->entry.not_inline) {
      return cold->entry.po_ptr.fileoff.bytes != cnew->entry.po_ptr.fileoff.bytes ||
             cold->entry.po_ptr.offset.bytes != cnew->entry.po_ptr.offset.bytes;
    } else {
      return strcmp(cold->entry.name, cnew->entry.name);
    }
    break;
  }
  default:
    return true;
  }
}

static void tpt_erase_column(const struct tp_tuple *tpt_old,
                             const struct tp_tuple *tpt_new,
                             const struct dpt_column *dptc, size_t col_off,
                             struct dbms *dbms) {
  void *col_old = (void *)tpt_old + col_off;
  void *col_new = tpt_new ? (void *)tpt_new + col_off : NULL;

  if (tpt_should_erase_column(col_old, col_new, dptc->type)) {
    switch (dptc->type) {

    case COLUMN_TYPE_STRING: {
      struct TPT_COL_TYPE(COLUMN_TYPE_STRING) * cold;
      cold = col_old;
      dbms_sso_remove(&cold->entry, dbms);
      break;
    }

    default: {
      assert(false && "tpt_update_column: Unknown inlined type.\n");
      break;
    }
    }
  }
}

void tpt_erase(const struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
               const struct dp_tuple *dpt, const struct tpt_col_info *col_info,
               struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    tpt_erase_column(tpt_old, tpt_new, &dpt->columns[i], col_info[i].start, dbms);
  }
}

void tpt_erase_columns(struct tp_tuple *tpt_old, struct tp_tuple *tpt_new,
                       const struct dp_tuple *dpt, const struct tpt_col_info *col_info,
                       size_t arr_size, size_t *idxs, struct dbms *dbms) {
  for (size_t i = 0; i < arr_size; ++i) {
    size_t idx = idxs[i];
    tpt_erase_column(tpt_old, tpt_new, &dpt->columns[idx], col_info[idx].start, dbms);
  }
}
