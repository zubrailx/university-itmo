#include "database.h"

#include <malloc.h>
#include <string.h>

// PAGE
struct database_page *dp_construct(struct pageoff_t size) {
  return (database_page *)page_construct(size, PAGE_DATABASE);
}

void dp_destruct(struct database_page **page_ptr) {
  page_destruct((base_page **)page_ptr);
}

// ITERATOR
static pageoff_t dp_get_next_index_pageoff(const struct database_page *page,
                                           pageoff_t cur) {
  while (cur.bytes > page->header.index_start.bytes) {
    cur = get_pageoff_t(cur.bytes - sizeof(page_index));
    page_index *idx = (void *)page + cur.bytes;
    if (idx->is_present) {
      break;
    }
  }
  return cur;
}

static inline bool dp_iter_is_end(dp_typle_iter *it) {
  return it->icur.bytes == it->iend.bytes;
}

struct dp_typle_iter *dp_typle_iter_construct(struct database_page *page) {
  dp_typle_iter *iter = my_malloc(dp_typle_iter);
  if (page != NULL) {
    *iter = (dp_typle_iter){
        .page = page,
        .icur = dp_get_next_index_pageoff(page, page->header.base.size),
        .iend = get_pageoff_t(page->header.index_start.bytes - sizeof(page_index))};
  } else {
    // main info - icur = iend
    *iter = (dp_typle_iter){
        .page = page, .icur = get_pageoff_t(0), .iend = get_pageoff_t(0)};
  }
  return iter;
}

void dp_typle_iter_destruct(struct dp_typle_iter **it_ptr) {
  if (*it_ptr) {
    free(*it_ptr);
  }
}

bool dp_typle_iter_next(struct dp_typle_iter *it) {
  it->icur = dp_get_next_index_pageoff(it->page, it->icur);
  return !dp_iter_is_end(it);
}

struct dp_typle *dp_typle_iter_get(struct dp_typle_iter *it) {
  return dp_iter_is_end(it) ? NULL : (void *)it->page + it->icur.bytes;
}

size_t dp_space_left(const struct database_page *page) {
  return (page->header.index_start.bytes - page->header.typle_end.bytes);
}

size_t dp_typle_size(size_t columns) {
  return offsetof(dp_typle, columns) + sizeof(dpt_column) * columns;
}

// insert typle + index
pageoff_t dp_insert_typle(struct database_page *page, dp_typle *typle) {
  size_t size = dp_typle_size(typle->header.cols);
  if (dp_space_left(page) < size + sizeof(page_index)) {
    return get_pageoff_t(0);
  } else {
    pageoff_t idx_off_start =
        get_pageoff_t(page->header.index_start.bytes - sizeof(page_index));
    pageoff_t tpl_off_start = page->header.typle_end;

    page_index index = (page_index){.is_present = true,
                                    .start = tpl_off_start,
                                    .end = get_pageoff_t(tpl_off_start.bytes + size)};

    void *idx_ptr = (void *)page + idx_off_start.bytes;
    memcpy(idx_ptr, &index, sizeof(page_index));
    void *tpl_ptr = (void *)page + tpl_off_start.bytes;
    memcpy(tpl_ptr, typle, size);
    // return idx start
    return idx_off_start;
  }
}
