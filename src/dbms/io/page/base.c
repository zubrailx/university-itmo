#include "base.h"

#include <malloc.h>

base_page *page_construct(const struct pageoff_t size, enum page_types type) {
  base_page *page = (base_page *)calloc(size.bytes, 1);
  page->header.size = size;
  page->header.type = (uint8_t)type;
  return page;
}

void page_destruct(struct base_page **page_ptr) {
  free((void *)*page_ptr);
  *page_ptr = NULL;
}
