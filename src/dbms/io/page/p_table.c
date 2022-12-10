#include "p_table.h"

#include "p_database.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(struct table_page, body, tp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(struct table_page, body, tp)

static void tcur_next(struct pageoff_t *cur, size_t tuple_size) {
  cur->bytes += tuple_size;
}

static pageoff_t tcur_end(const struct table_page *page) {
  return page->header.tuple_barrier;
}

static tp_tuple *tuple_locate(const struct table_page *page, pageoff_t cur) {
  return (void *)page + cur.bytes;
}

static size_t tuple_count_slot(const pageoff_t page_size, const size_t tuple_size) {
  return tp_page_body(page_size).bytes / (tuple_size + sizeof(slot_index));
}

// Currently calculate size using switch case
static size_t get_column_size(uint8_t type) {
  switch (type) {
  case COLUMN_TYPE_DOUBLE:
    return TPT_COLUMN_SIZE(COLUMN_TYPE_DOUBLE);
  case COLUMN_TYPE_INT32:
    return TPT_COLUMN_SIZE(COLUMN_TYPE_INT32);
  case COLUMN_TYPE_BOOL:
    return TPT_COLUMN_SIZE(COLUMN_TYPE_BOOL);
  case COLUMN_TYPE_STRING:
    return TPT_COLUMN_SIZE(COLUMN_TYPE_STRING);
  default:
    assert(0 && "ERROR: Unknown column type");
  }
}

// Push and pop slots
static void slot_push(table_page *page, const slot_index slot) {
  page->header.index_start.bytes -= sizeof(slot_index);
  *(slot_index *)((void *)page + page->header.index_start.bytes) = slot;
}

static slot_index slot_pop(table_page *page) {
  slot_index slot = *(slot_index *)((void *)page + page->header.index_start.bytes);
  page->header.index_start.bytes += sizeof(slot_index);
  return slot;
}

pageoff_t tp_get_min_size(const struct dp_tuple *tuple) {
  size_t tuple_size = tp_get_tuple_size(tuple);
  return get_pageoff_t(offsetof(table_page, body) + tuple_size + sizeof(slot_index));
}

size_t tp_get_tuple_size(const struct dp_tuple *tuple) {
  size_t cols = tuple->header.cols;
  size_t size = offsetof(struct tp_tuple, columns);
  for (size_t i = 0; i < cols; ++i) {
    size += get_column_size(tuple->columns[i].type);
  }
  return size;
}

tpt_col_info *tp_constuct_col_info_arr(const dp_tuple *tuple) {
  size_t cols = tuple->header.cols;

  tpt_col_info *col_info = malloc(sizeof(tpt_col_info) * cols);

  size_t off_cur = offsetof(struct tp_tuple, columns);
  for (size_t i = 0; i < cols; ++i) {
    col_info[i] = (tpt_col_info){.start = off_cur};
    off_cur += get_column_size(tuple->columns[i].type);
  }
  return col_info;
}

// FULL/EMPTY
bool tp_is_full(const struct table_page *page) {
  return page->header.index_start.bytes == page->header.base.size.bytes;
}

bool tp_is_empty(const struct table_page *page) {
  return page->header.index_start.bytes == page->header.index_barrier.bytes;
}

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(struct table_page, tp, PAGE_TABLE)
PAGE_DESTRUCT_DEFAULT_IMPL(struct table_page, tp)

struct table_page *tp_construct_init(const struct pageoff_t size, const fileoff_t prev,
                                     const fileoff_t next, const dp_tuple *tuple) {
  // check if at least one row can be inserted
  const pageoff_t min_size = tp_get_min_size(tuple);
  assert(size.bytes >= min_size.bytes && min_size.bytes && "Too small size of page");

  table_page *page = tp_construct(size);
  page->header.next = next;
  page->header.prev = prev;

  const size_t tuple_size = tp_get_tuple_size(tuple);
  const size_t slots = tuple_count_slot(size, tuple_size);

  page->header.index_barrier = get_pageoff_t(size.bytes - sizeof(slot_index) * slots);
  page->header.tuple_barrier = tp_body_page(get_bodyoff_t(tuple_size * slots));
  page->header.index_start = size;

  // push all empty slots
  slot_index cur_slot = {get_pageoff_t(page->header.tuple_barrier.bytes - tuple_size)};

  for (size_t i = 0; i < slots; ++i) {
    slot_push(page, cur_slot);
    cur_slot.start.bytes -= tuple_size;
  }
  // TEST:
  // printf("%u : ", page->header.index_barrier.bytes);
  // printf("%u", page->header.index_start.bytes);
  return page;
}

// don't check if page is full
pageoff_t tp_insert_row(struct table_page *page, const tp_tuple *tuple,
                        size_t tuple_size) {
  pageoff_t off = slot_pop(page).start;
  void *dest_ptr = (void *)page + off.bytes;
  memcpy(dest_ptr, tuple, tuple_size);
  return off;
}
// void tp_remove_row(struct table_page *page, const tp_tuple *tuple, size_t tuple_size)
// void tp_update_row(struct table_page *page, const tp_tuple *tuple, size_t
// tuple_size);

// Iterators
static pageoff_t next_tuple(const struct table_page *page, pageoff_t cur,
                            size_t tuple_size) {
  tcur_next(&cur, tuple_size);
  for (; cur.bytes < tcur_end(page).bytes; tcur_next(&cur, tuple_size)) {
    tp_tuple *tuple = tuple_locate(page, cur);
    if (tuple->header.is_present) {
      return cur;
    }
  }
  return tcur_end(page);
}

static inline bool iter_is_end(tp_tuple_iter *it) {
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
  it->tcur = next_tuple(it->page, it->tcur, it->tuple_size);
  return !iter_is_end(it);
}
struct tp_tuple *tp_tuple_iter_get(struct tp_tuple_iter *it) {
  if (iter_is_end(it))
    return NULL;
  return tuple_locate(it->page, it->tcur);
}
