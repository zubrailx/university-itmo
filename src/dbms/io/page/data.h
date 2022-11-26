#pragma once

#include "base.h"

#include <stddef.h>

typedef struct data_header {
  struct base_header base;
  bodyoff_t stored_last;
} data_header;

typedef struct data_page {
  struct data_header header;
  uint8_t body[];
} __attribute__((packed)) data_page;

// Bodyoff/pageoff
BODYOFF_TO_PAGEOFF(data_page, body, da)
PAGEOFF_TO_BODYOFF(data_page, body, da)

struct data_page *da_construct(struct pageoff_t size);
void da_destruct(struct data_page **page_ptr);
size_t da_space_left(const struct data_page *page);
pageoff_t da_insert_data(struct data_page *page, const void *data, size_t size);
