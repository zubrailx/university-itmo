#include "p_container.h"

#include <stddef.h>

PAGE_CONSTRUCT_DEFAULT_IMPL(page_container, container, PAGE_CONTAINER)
PAGE_DESTRUCT_DEFAULT_IMPL(page_container, container)

page_container *container_construct_init(pageoff_t size, fileoff_t prev,
                                         fileoff_t next) {
  page_container *container = container_construct(size);
  container->header.end = get_pageoff_t(offsetof(struct page_container, body));
  container->header.next = next;
  container->header.prev = prev;

  return container;
}

static bool container_is_full(const struct page_container *page) {
  return page->header.base.size.bytes <
         page->header.end.bytes + sizeof(struct page_entry);
}

bool container_empty(const struct page_container *page) {
  return page->header.end.bytes == offsetof(struct page_container, body);
}

bool container_push(struct page_container *page, struct page_entry *entry) {
  if (!container_is_full(page)) {
    page_entry *ptr = (void *)page + page->header.end.bytes;
    *ptr = *entry;
    return true;
  }
  return false;
}

struct page_entry *container_pop(struct page_container *page) {
  if (container_empty(page)) {
    return NULL;
  } else {
    page->header.end.bytes -= sizeof(struct page_entry);
    page_entry *entry = (void *)page + page->header.end.bytes;
    return entry;
  }
}
struct page_entry *container_top(struct page_container *page) {
  if (container_empty(page)) {
    return NULL;
  } else {
    return (void *)page + page->header.end.bytes - sizeof(struct page_entry);
  }
}
