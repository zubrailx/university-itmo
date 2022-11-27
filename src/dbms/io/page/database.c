#include "database.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(database_page, body, dp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(database_page, body, dp)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(database_page, dp, PAGE_DATABASE)
PAGE_DESTRUCT_DEFAULT_IMPL(database_page, dp)

struct database_page *dp_construct_init(struct pageoff_t size, fileoff_t prev,
                                        fileoff_t next) {
  database_page *page = dp_construct(size);
  page->header.index_start = page->header.base.size;
  page->header.typle_end = get_pageoff_t(offsetof(database_page, body));
  page->header.next = next;
  page->header.prev = prev;
  return page;
}

size_t dp_space_left(const struct database_page *page) {
  return (page->header.index_start.bytes - page->header.typle_end.bytes);
}

size_t dp_typle_size(size_t columns) {
  return offsetof(dp_typle, columns) + sizeof(dpt_column) * columns;
}

static dp_typle *dp_typle_locate(const struct database_page *page,
                                 const pageoff_t idx_pageoff) {
  assert(page->header.base.size.bytes >= idx_pageoff.bytes + sizeof(page_index));
  assert(page->header.index_start.bytes <= idx_pageoff.bytes);

  page_index *index = (void *)page + idx_pageoff.bytes;
  return (void *)page + index->start.bytes;
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

    page_index index = (page_index){.start = tpl_off_start,
                                    .end = get_pageoff_t(tpl_off_start.bytes + size)};
    {// add information that is relevant only when in file
      typle->header.is_present = true;
    }
    void *idx_ptr = (char *)page + idx_off_start.bytes;
    memcpy(idx_ptr, &index, sizeof(page_index));
    void *tpl_ptr = (char *)page + tpl_off_start.bytes;
    memcpy(tpl_ptr, typle, size);

    // update page header
    page->header.index_start = idx_off_start;
    page->header.typle_end = index.end;
    // return idx start
    return idx_off_start;
  }
}

bool dp_drop_table(struct database_page *page, const pageoff_t pageoff) {
  dp_typle *typle = dp_typle_locate(page, pageoff);
  if (typle->header.is_present) {
    size_t size = dp_typle_size(typle->header.cols);
    memset(typle, 0, size);
    return true;
  }
  return false;
}

static void icur_next(pageoff_t *cur) { cur->bytes = cur->bytes - sizeof(page_index); }

static pageoff_t icur_end(const struct database_page *page) {
  return get_pageoff_t(page->header.index_start.bytes - sizeof(page_index));
}

// ITERATOR
static pageoff_t dp_get_next_index_pageoff(const struct database_page *page,
                                           pageoff_t cur) {
  // point to the next element
  icur_next(&cur);
  for (; cur.bytes >= page->header.index_start.bytes; icur_next(&cur)) {
    dp_typle *typle = dp_typle_locate(page, cur);
    if (typle->header.is_present) {
      return cur;
    }
  }
  return icur_end(page);
}

static inline bool dp_iter_is_end(dp_typle_iter *it) {
  return it->icur.bytes == it->iend.bytes;
}

struct dp_typle_iter *dp_typle_iter_construct(struct database_page *page) {
  dp_typle_iter *iter = my_malloc(dp_typle_iter);
  if (page != NULL) {
    pageoff_t first = page->header.base.size;
    *iter = (dp_typle_iter){.page = page,
                            .icur = dp_get_next_index_pageoff(page, first),
                            .iend = icur_end(page)};// points over the last element
  } else {
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
  if (dp_iter_is_end(it))
    return NULL;
  return dp_typle_locate(it->page, it->icur);
}
