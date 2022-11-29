#include "p_table.h"

#include "p_database.h"
#include <assert.h>
#include <stdlib.h>

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(table_page, body, tp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(table_page, body, tp)

EXTERN_INLINE_TPT_COLUMN(double, COLUMN_TYPE_DOUBLE)
EXTERN_INLINE_TPT_COLUMN(int32_t, COLUMN_TYPE_INT32)
EXTERN_INLINE_TPT_COLUMN(bool, COLUMN_TYPE_BOOL)
EXTERN_INLINE_TPT_COLUMN(page_sso, COLUMN_TYPE_STRING)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(table_page, tp, PAGE_TABLE)
PAGE_DESTRUCT_DEFAULT_IMPL(table_page, tp)

struct table_page *tp_construct_init(struct pageoff_t size, fileoff_t prev,
                                     fileoff_t next) {
  table_page *page = tp_construct(size);
  page->header.next = next;
  page->header.prev = prev;
  page->header.typle_end = get_pageoff_t(offsetof(table_page, body));
  return page;
}

static void tcur_next(struct pageoff_t *cur, size_t typle_size) {
  cur->bytes += typle_size;
}

static pageoff_t tcur_end(const struct table_page *page) {
  return page->header.typle_end;
}

static tp_typle *tp_typle_locate(const struct table_page *page, pageoff_t cur) {
  return (void *)page + cur.bytes;
}

// Currently calculate size using switch case
static size_t tpt_column_size(uint8_t type) {
  switch (type) {
  case COLUMN_TYPE_DOUBLE:
    return _TPT_COLUMN_SIZE(COLUMN_TYPE_DOUBLE);
  case COLUMN_TYPE_INT32:
    return _TPT_COLUMN_SIZE(COLUMN_TYPE_INT32);
  case COLUMN_TYPE_BOOL:
    return _TPT_COLUMN_SIZE(COLUMN_TYPE_BOOL);
  case COLUMN_TYPE_STRING:
    return _TPT_COLUMN_SIZE(COLUMN_TYPE_STRING);
  default:
    assert(0 && "ERROR: Unknown column type");
  }
}

size_t tp_get_typle_size(const struct dp_typle *typle) {
  size_t cols = typle->header.cols;
  size_t size = offsetof(struct tp_typle, columns);
  for (size_t i = 0; i < cols; ++i) {
    size += tpt_column_size(typle->columns[i].type);
  }
  return size;
}

// Iterators
static pageoff_t tp_get_next_typle(const struct table_page *page, pageoff_t cur,
                                   size_t typle_size) {
  tcur_next(&cur, typle_size);
  for (; cur.bytes < tcur_end(page).bytes; tcur_next(&cur, typle_size)) {
    tp_typle *typle = tp_typle_locate(page, cur);
    if (typle->header.is_present) {
      return cur;
    }
  }
  return tcur_end(page);
}

static inline bool tp_iter_is_end(tp_typle_iter *it) {
  return it->tcur.bytes == it->tend.bytes;
}

struct tp_typle_iter *tp_typle_iter_construct(struct table_page *page,
                                              size_t typle_size) {
  tp_typle_iter *iter = my_malloc(tp_typle_iter);
  if (page != NULL) {
    *iter = (tp_typle_iter){.page = page,
                            .tcur = get_pageoff_t(offsetof(table_page, body)),
                            .tend = page->header.typle_end,
                            .typle_size = typle_size};
  } else {
    *iter = (tp_typle_iter){
        .page = page, .tcur = get_pageoff_t(0), .tend = get_pageoff_t(0)};
  }
  return iter;
}

void tp_typle_iter_destruct(struct tp_typle_iter **it_ptr) {
  if (*it_ptr) {
    free(*it_ptr);
  }
}

bool tp_typle_iter_next(struct tp_typle_iter *it) {
  it->tcur = tp_get_next_typle(it->page, it->tcur, it->typle_size);
  return !tp_iter_is_end(it);
}
struct tp_typle *tp_typle_iter_get(struct tp_typle_iter *it) {
  if (tp_iter_is_end(it))
    return NULL;
  return tp_typle_locate(it->page, it->tcur);
}
