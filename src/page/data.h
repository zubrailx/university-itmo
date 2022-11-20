#pragma once

#include "../util/define.h"
#include "base.h"

my_defstruct(data_header);
struct data_header {
  struct base_header base;
  bodyoff_t stored_last;
};

my_defstruct(data_page);
struct data_page {
  struct data_header header;
  uint8_t body[];
} __attribute__((packed));

// Bodyoff/pageoff
BODYOFF_TO_PAGEOFF(data_header, da)
PAGEOFF_TO_BODYOFF(data_header, da)

struct data_page *da_construct(struct pageoff_t size);
void da_destruct(struct data_page **page_ptr);
