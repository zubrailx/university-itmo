#pragma once

#include <inttypes.h>

#include "../../util/define.h"
#include "../internals.h"

my_defstruct(base_header);
struct base_header {
  uint8_t type;
  struct page_size size;
};

my_defstruct(base_page);
struct base_page {
  struct base_header header;
} __attribute__((packed));

enum page_types {
  PAGE_UNKNOWN,
  PAGE_DATABASE,
  PAGE_HASHTABLE,
  PAGE_BACKET,
  PAGE_PAGE,
  PAGE_DATA,
  PAGE_TABLE,
  PAGE_TMP,
  PAGE_DUMPED,
};

base_page *page_construct(const struct page_size size, enum page_types type);
void page_destruct(struct base_page **page_ptr);
