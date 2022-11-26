#include "data.h"

#include <string.h>
#include <util/internals.h>

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(data_page, body, da)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(data_page, body, da)

PAGE_CONSTRUCT_DEFAULT_IMPL(data_page, da, PAGE_DATA)
PAGE_DESTRUCT_DEFAULT_IMPL(data_page, da)

struct data_page *da_construct_init(struct pageoff_t size) {
  data_page *page = da_construct(size);
  page->header.stored_end = get_pageoff_t(offsetof(data_page, body));
  return page;
}

size_t da_space_left(const struct data_page *page) {
  return (page->header.base.size.bytes - page->header.stored_end.bytes);
}

// If returns 0 then no available space left
pageoff_t da_insert_data(struct data_page *page, const void *data, size_t size) {
  if (da_space_left(page) < size) {
    return get_pageoff_t(0);
  } else {
    pageoff_t ins_off = page->header.stored_end;
    void *ins_pos = (char *)page + ins_off.bytes;

    memcpy(ins_pos, data, size);
    page->header.stored_end = get_pageoff_t(ins_off.bytes + size);
    return ins_off;
  }
}
