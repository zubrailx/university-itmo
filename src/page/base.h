#pragma once

#include <inttypes.h>

#include "../util/define.h"
#include "internals.h"

my_defstruct(base_header);
struct base_header {
  uint8_t type;
  struct pageoff_t size;
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

base_page *page_construct(const struct pageoff_t size, enum page_types type);
void page_destruct(struct base_page **page_ptr);

#define BODYOFF_TO_PAGEOFF(header_type, prefix)                                        \
  inline pageoff_t prefix##_bodyoff_to_pageoff(bodyoff_t bodyoff) {                    \
    return get_pageoff_t(bodyoff.bytes + sizeof(header_type));                         \
  }
#define PAGEOFF_TO_BODYOFF(header_type, prefix)                                        \
  inline bodyoff_t prefix##_pageoff_to_bodyoff(pageoff_t pageoff) {                    \
    return get_bodyoff_t(pageoff.bytes - sizeof(header_type));                         \
  }
