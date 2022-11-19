#pragma once

#include "../util/types.h"
#include "base.h"
#include "base_io.h"

extern size_t DATABASE_PAGE_SIZE;

struct dbfile;

my_defstruct(database_header);
struct database_header {
  struct base_page base;
  fileoff_t next;
  fileoff_t previous;
  /* Non-inclusive body offset */
  bodyoff_t index_last;
  /* From start to end */
  bodyoff_t typle_start;
};

my_defstruct(database_page);
struct database_page {
  struct database_header header;
  uint8_t body[];
} __attribute__((packed));

inline struct page_size dp_size_from_cap(struct page_cap cap) {
  return (struct page_size){.bytes = cap.bytes + offsetof(database_page, body)};
}

inline struct page_cap dp_cap_from_size(struct page_size size) {
  return (struct page_cap){.bytes = size.bytes + offsetof(database_page, body)};
}

// RAM
struct database_page *dp_construct(struct page_size size);
void dp_destruct(struct database_page **page_ptr);

// FILE
PAGE_DEFAULT_CREATE(struct database_page, dp)
PAGE_DEFAULT_ALTER(struct database_page, dp)
PAGE_DEFAULT_DROP(struct database_page, dp)
