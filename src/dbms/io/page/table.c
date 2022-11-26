#include "table.h"

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(table_page, body, tp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(table_page, body, tp)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(table_page, tp, PAGE_TABLE)
PAGE_DESTRUCT_DEFAULT_IMPL(table_page, tp)

struct table_page *tp_construct_init(struct pageoff_t size, fileoff_t prev,
                                     fileoff_t next) {
  table_page *page = tp_construct(size);
  page->header.next = next;
  page->header.prev = prev;
  page->header.typle_end = get_pageoff_t(offsetof(table_page, body));
  return page;
}
