#pragma once

#include "base.h"

typedef struct data_header {
  struct base_header base;
  bodyoff_t stored_last;
} data_header;

typedef struct data_page {
  struct data_header header;
  uint8_t body[];
} __attribute__((packed)) data_page;

// Bodyoff/pageoff
BODYOFF_TO_PAGEOFF(data_header, da)
PAGEOFF_TO_BODYOFF(data_header, da)

struct data_page *da_construct(struct pageoff_t size);
void da_destruct(struct data_page **page_ptr);
