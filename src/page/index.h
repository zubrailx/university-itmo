#pragma once

#include <stddef.h>

#include "../util/define.h"
#include "internals.h"

my_defstruct(page_index);
struct page_index {
  pageoff_t start;
  pageoff_t end;
  bool is_present;
};

inline void *page_index_get_ptr(void *page, struct page_index *index) {
  return page + index->start.bytes;
}
inline size_t page_index_get_size(struct page_index *index) {
  return index->end.bytes - index->start.bytes;
}
