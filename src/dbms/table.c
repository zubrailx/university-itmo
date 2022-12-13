#include "io/page/p_database.h"
#include "plan.h"
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

static bool dbms_sso_do_update(const page_sso *lsv, const page_sso *rsv) {
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

static bool type_can_not_inline(uint8_t type) { return type == COLUMN_TYPE_STRING; }

static void tpt_update_column(const struct tp_tuple *tpt_old,
                              const struct tp_tuple *tpt_new,
                              const struct dpt_column *dptc, size_t col_off,
                              struct dbms *dbms) {
  if (type_can_not_inline(dptc->type)) {
    void *col_old_p = (void *)tpt_old + col_off;
    void *col_new_p = (void *)tpt_new + col_off;
    switch (dptc->type) {
    case COLUMN_TYPE_STRING: {
      struct TPT_COL_TYPE(COLUMN_TYPE_STRING) * col_old, *col_new;
      col_old = col_old_p;
      col_new = col_new_p;
      if (dbms_sso_do_update(&col_old->entry, &col_new->entry)) {
        dbms_sso_remove(&col_old->entry, dbms);
      }
      break;
    }
    default:
      assert(false && "tpt_update_column: Unknown inlined type.\n");
      break;
    }
  }
}

static void tpt_remove_column(const struct tp_tuple *tpt_old,
                              const struct dpt_column *dptc, size_t col_off,
                              struct dbms *dbms) {
  if (type_can_not_inline(dptc->type)) {
    void *col_old_p = (void *)tpt_old + col_off;
    switch (dptc->type) {
    case COLUMN_TYPE_STRING: {
      struct TPT_COL_TYPE(COLUMN_TYPE_STRING) * col_old;
      col_old = col_old_p;
      dbms_sso_remove(&col_old->entry, dbms);
      break;
    }
    default:
      assert(false && "tpt_update_column: Unknown inlined type.\n");
      break;
    }
  }
}

void tpt_update(struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
                const struct dp_tuple *dpt, struct tpt_col_info *col_info,
                struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    tpt_update_column(tpt_old, tpt_new, &dpt->columns[i], col_info[i].start, dbms);
  }
}

void tpt_update_columns(struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
                        const struct dp_tuple *dpt, const struct tpt_col_info *col_info,
                        size_t arr_size, size_t *idxs, struct dbms *dbms) {
  for (size_t i = 0; i < arr_size; ++i) {
    size_t col_idx = idxs[i];
    tpt_update_column(tpt_old, tpt_new, &dpt->columns[col_idx], col_info[col_idx].start,
                      dbms);
  }
}

void tpt_remove(const struct tp_tuple *tpt_old, const struct dp_tuple *dpt,
                const struct tpt_col_info *col_info, struct dbms *dbms) {
  for (size_t i = 0; i < dpt->header.cols; ++i) {
    tpt_remove_column(tpt_old, &dpt->columns[i], col_info[i].start, dbms);
  }
}

void tpt_remove_columns(struct tp_tuple *tpt_old, const struct dp_tuple *dpt,
                        const struct tpt_col_info *col_info, size_t arr_size,
                        size_t *idxs, struct dbms *dbms) {
  for (size_t i = 0; i < arr_size; ++i) {
    size_t col_idx = idxs[i];
    tpt_remove_column(tpt_old, &dpt->columns[col_idx], col_info[col_idx].start, dbms);
  }
}
