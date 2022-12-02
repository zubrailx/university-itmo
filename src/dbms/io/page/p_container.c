#include "p_container.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <util/define.h>

PAGE_CONSTRUCT_DEFAULT_IMPL(page_container, container, PAGE_CONTAINER)
PAGE_DESTRUCT_DEFAULT_IMPL(page_container, container)

page_container *container_construct_init(pageoff_t size, fileoff_t prev) {
  page_container *container = container_construct(size);
  container->header.start = container->header.base.size;
  container->header.prev = prev;

  return container;
}

bool container_full(const struct page_container *page) {
  return offsetof(page_container, body) > page->header.start.bytes - sizeof(page_entry);
}

bool container_empty(const struct page_container *page) {
  return page->header.start.bytes == page->header.base.size.bytes;
}

bool container_push(struct page_container *page, const struct page_entry *entry) {
  if (!container_full(page)) {
    page->header.start.bytes -= sizeof(page_entry);
    page_entry *ptr = (void *)page + page->header.start.bytes;
    *ptr = *entry;
    return true;
  }
  return false;
}

struct page_entry *container_pop(struct page_container *page) {
  if (container_empty(page)) {
    return NULL;
  } else {
    page_entry *entry = (void *)page + page->header.start.bytes;
    page->header.start.bytes += sizeof(struct page_entry);
    return entry;
  }
}
struct page_entry *container_top(const struct page_container *page) {
  if (container_empty(page)) {
    return NULL;
  } else {
    return (void *)page + page->header.start.bytes;
  }
}

extern inline page_entry page_entry_construct(fileoff_t fileoff);

// Iterator over entries
static void cur_next(pageoff_t *cur) { cur->bytes = cur->bytes + sizeof(page_entry); }

static pageoff_t cur_end(const page_container *page) { return page->header.base.size; }

static pageoff_t cp_get_next_index_pageoff(const struct page_container *page,
                                           pageoff_t cur) {
  cur_next(&cur);
  pageoff_t end = cur_end(page);
  return cur.bytes >= end.bytes ? end : cur;
}

static inline bool cp_iter_is_end(cp_entry_iter *it) {
  return it->cur.bytes == it->end.bytes;
}

struct cp_entry_iter *cp_entry_iter_construct(struct page_container *page) {

  cp_entry_iter *iter = my_malloc(cp_entry_iter);
  if (page != NULL) {
    pageoff_t first = page->header.start;
    *iter = (cp_entry_iter){.page = page,
                            .cur = first,
                            .end = cur_end(page)};// points over the last element
  } else {
    *iter =
        (cp_entry_iter){.page = page, .cur = get_pageoff_t(0), .end = get_pageoff_t(0)};
  }
  return iter;
}

void cp_entry_iter_destruct(struct cp_entry_iter **it_ptr) {
  if (*it_ptr) {
    free(*it_ptr);
  }
}

bool cp_entry_iter_next(struct cp_entry_iter *it) {
  it->cur = cp_get_next_index_pageoff(it->page, it->cur);
  return !cp_iter_is_end(it);
}

struct page_entry *cp_entry_iter_get(struct cp_entry_iter *it) {
  if (cp_iter_is_end(it))
    return NULL;
  return (void *)it->page + it->cur.bytes;
}
