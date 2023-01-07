#pragma once

#include <stddef.h>

#include <dbms/util/define.h>
#include <dbms/internals/po_ptr.h>

typedef struct page_index {
  pageoff_t start;
  pageoff_t end;
} page_index;

inline void *page_index_get_ptr(void *page, struct page_index *index) {
  return (char *)page + index->start.bytes;
}
inline size_t page_index_get_size(struct page_index *index) {
  return index->end.bytes - index->start.bytes;
}
