#include "database.h"

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

static inline bool dp_iterator_is_end(dp_typle_iter *it) {
  return it->icur.bytes == it->iend.bytes;
}

struct dp_typle_iter dp_get_typle_iterator(struct database_page *page) {
  return (dp_typle_iter){
      .page = page,
      .icur = dp_get_next_index_pageoff(page, page->header.base.size),
      .iend = get_pageoff_t(page->header.index_start.bytes - sizeof(page_index))};
}

bool dp_typle_iter_next(struct dp_typle_iter *it) {
  it->icur = dp_get_next_index_pageoff(it->page, it->icur);
  return !dp_iterator_is_end(it);
}

struct dp_typle *dp_typle_iter_get(struct dp_typle_iter *it) {
  return dp_iterator_is_end(it) ? NULL : (void *)it->page + it->icur.bytes;
}
