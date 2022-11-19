#pragma once

#include "../../util/define.h"
#include "../base/base.h"
#include "../base/base_io.h"

my_defstruct(data_header);
struct data_header {
  struct base_page base;
  bodyoff_t stored_last;
};

my_defstruct(data_page);
struct data_page {
  struct data_header header;
  uint8_t body[];
} __attribute__((packed));

// RAM
struct data_page *da_construct(struct page_size size);
void da_destruct(struct data_page **page_ptr);

// Operations

// FILE
PAGE_DEFAULT_CREATE(struct data_page, da)
PAGE_DEFAULT_ALTER(struct data_page, da)
PAGE_DEFAULT_DROP(struct data_page, da)
