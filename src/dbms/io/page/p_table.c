#include "p_table.h"

#include "p_database.h"
#include <assert.h>
#include <stdio.h>
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
  return tp_page_body(page_size).bytes / (tuple_size + sizeof(slot_index));
}

static bool tp_tuple_full(const struct table_page *page) {
  return page->header.index_start.bytes == page->header.base.size.bytes;
}

static bool tp_tuple_empty(const struct table_page *page) {
  return page->header.index_start.bytes == page->header.index_barrier.bytes;
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
  return get_pageoff_t(offsetof(table_page, body) + tuple_size + sizeof(slot_index));
}

size_t tp_get_tuple_size(const struct dp_tuple *tuple) {
  size_t cols = tuple->header.cols;
  size_t size = offsetof(struct tp_tuple, columns);
  for (size_t i = 0; i < cols; ++i) {
    size += tpt_column_size(tuple->columns[i].type);
  }
  return size;
}

// Push and pop slots
static void tp_slot_push(table_page *page, const slot_index slot) {
  page->header.index_start.bytes -= sizeof(slot_index);
  *(slot_index *)((void *)page + page->header.index_start.bytes) = slot;
}

static slot_index tp_slot_pop(table_page *page) {
  slot_index slot = *(slot_index *)((void *)page + page->header.index_start.bytes);
  page->header.index_start.bytes += sizeof(slot_index);
  return slot;
}

// Constructor implementations
struct table_page *tp_construct_init(const struct pageoff_t size, const fileoff_t prev,
                                     const fileoff_t next, const dp_tuple *tuple) {
  // check if at least one row can be inserted
  const pageoff_t min_size = tp_get_min_size(tuple);
  assert(size.bytes >= min_size.bytes && min_size.bytes && "Too small size of page");

  table_page *page = tp_construct(size);
  page->header.next = next;
  page->header.prev = prev;

  const size_t tuple_size = tp_get_tuple_size(tuple);
  const size_t slots = tp_tuple_slot_count(size, tuple_size);

  page->header.index_barrier = get_pageoff_t(size.bytes - sizeof(slot_index) * slots);
  page->header.tuple_barrier = tp_body_page(get_bodyoff_t(tuple_size * slots));
  page->header.index_start = size;

  // push all empty slots
  slot_index cur_slot = {get_pageoff_t(page->header.tuple_barrier.bytes - tuple_size)};

  for (size_t i = 0; i < slots; ++i) {
    tp_slot_push(page, cur_slot);
    cur_slot.start.bytes -= tuple_size;
  }
  printf("%u : ", page->header.index_barrier.bytes);
  printf("%u", page->header.index_start.bytes);
  return page;
}

pageoff_t tp_insert_row(struct table_page *page, const tp_tuple *tuple,
                        size_t tuple_size);
void tp_remove_row(struct table_page *page, const tp_tuple *tuple, size_t tuple_size);
void tp_update_row(struct table_page *page, const tp_tuple *tuple, size_t tuple_size);
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
