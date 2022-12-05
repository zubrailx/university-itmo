#include "p_table.h"

#include "p_database.h"
#include <assert.h>
#include <stdlib.h>

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(struct table_page, body, tp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(struct table_page, body, tp)

EXTERN_INLINE_TPT_COLUMN(double, COLUMN_TYPE_DOUBLE)
EXTERN_INLINE_TPT_COLUMN(int32_t, COLUMN_TYPE_INT32)
EXTERN_INLINE_TPT_COLUMN(bool, COLUMN_TYPE_BOOL)
EXTERN_INLINE_TPT_COLUMN(page_sso, COLUMN_TYPE_STRING)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(struct table_page, tp, PAGE_TABLE)
PAGE_DESTRUCT_DEFAULT_IMPL(struct table_page, tp)

static void tcur_next(struct pageoff_t *cur, size_t tuple_size) {
  cur->bytes += tuple_size;
}

static pageoff_t tcur_end(const struct table_page *page) {
  return page->header.tuple_barrier;
}

static tp_tuple *tp_tuple_locate(const struct table_page *page, pageoff_t cur) {
  return (void *)page + cur.bytes;
}

static size_t tp_tuple_slot_count(const pageoff_t page_size, const size_t tuple_size) {
  return tp_page_body(page_size).bytes / (tuple_size + sizeof(empty_slot));
}

static bool tp_tuple_full(const struct table_page *page) {
  return page->header.empty_start.bytes == page->header.base.size.bytes;
}

static bool tp_tuple_empty(const struct table_page *page) {
  return page->header.empty_start.bytes == page->header.slot_barrier.bytes;
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

pageoff_t tp_get_min_size(const struct dp_tuple *tuple) {
  size_t tuple_size = tp_get_tuple_size(tuple);
  return get_pageoff_t(offsetof(table_page, body) + tuple_size + sizeof(empty_slot));
}

size_t tp_get_tuple_size(const struct dp_tuple *tuple) {
  size_t cols = tuple->header.cols;
  size_t size = offsetof(struct tp_tuple, columns);
  for (size_t i = 0; i < cols; ++i) {
    size += tpt_column_size(tuple->columns[i].type);
  }
  return size;
}

struct table_page *tp_construct_init(const struct pageoff_t size, const fileoff_t prev,
                                     const fileoff_t next, const dp_tuple *tuple) {
  // check if at least one row can be inserted
  const pageoff_t min_size = tp_get_min_size(tuple);
  assert(size.bytes >= min_size.bytes && min_size.bytes && "Too small size of page");

  table_page *page = tp_construct(size);
  page->header.next = next;
  page->header.prev = prev;

  size_t tuple_size = tp_get_tuple_size(tuple);
  size_t slots = tp_tuple_slot_count(size, tuple_size);

  page->header.slot_barrier = get_pageoff_t(size.bytes - sizeof(empty_slot) * slots);
  page->header.tuple_barrier = tp_body_page(get_bodyoff_t(tuple_size * slots));
  page->header.empty_start = size;

  // TODO: push all empty slots
  return page;
}

// Iterators
static pageoff_t tp_next_tuple(const struct table_page *page, pageoff_t cur,
                               size_t tuple_size) {
  tcur_next(&cur, tuple_size);
  for (; cur.bytes < tcur_end(page).bytes; tcur_next(&cur, tuple_size)) {
    tp_tuple *tuple = tp_tuple_locate(page, cur);
    if (tuple->header.is_present) {
      return cur;
    }
  }
  return tcur_end(page);
}

static inline bool tp_iter_is_end(tp_tuple_iter *it) {
  return it->tcur.bytes == it->tend.bytes;
}

struct tp_tuple_iter *tp_tuple_iter_construct(struct table_page *page,
                                              size_t tuple_size) {
  tp_tuple_iter *iter = my_malloc(tp_tuple_iter);
  if (page != NULL) {
    *iter = (tp_tuple_iter){.page = page,
                            .tcur = get_pageoff_t(offsetof(table_page, body)),
                            .tend = tcur_end(page),
                            .tuple_size = tuple_size};
  } else {
    *iter = (tp_tuple_iter){
        .page = page, .tcur = get_pageoff_t(0), .tend = get_pageoff_t(0)};
  }
  return iter;
}

void tp_tuple_iter_destruct(struct tp_tuple_iter **it_ptr) {
  if (*it_ptr) {
    free(*it_ptr);
  }
}

bool tp_tuple_iter_next(struct tp_tuple_iter *it) {
  it->tcur = tp_next_tuple(it->page, it->tcur, it->tuple_size);
  return !tp_iter_is_end(it);
}
struct tp_tuple *tp_tuple_iter_get(struct tp_tuple_iter *it) {
  if (tp_iter_is_end(it))
    return NULL;
  return tp_tuple_locate(it->page, it->tcur);
}
