#pragma once

#include <inttypes.h>

#include "../util/define.h"

// Size of page
my_defstruct(page_size);
struct page_size {
  uint32_t bytes;
};

// Size of body
my_defstruct(page_cap);
struct page_cap {
  uint32_t bytes;
};

#define pagesize(X)((page_size){.bytes = sizeof(X)})
