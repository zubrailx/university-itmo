#pragma once

#include "p_base.h"

#include <stddef.h>

/*
Structure: data_page
  |------------------|
  |HHHHHHH|TTTT|     | header.slot - size of header slot
  |------------------|
  |     |TTTTT|      |
  |------------------|
  |     |        IIII| I - empty slots
  |------------------|

*/

typedef struct data_header {
  struct base_header base;
  size_t slot_size;
  size_t slot_count;
  pageoff_t slot_start;
} data_header;

typedef struct data_page {
  struct data_header header;
  uint8_t body[];
} __attribute__((packed)) data_page;

// Bodyoff/pageoff
INLINE_BODYOFF_TO_PAGEOFF(data_page, body, da)
INLINE_PAGEOFF_TO_BODYOFF(data_page, body, da)

data_page *da_construct(const size_t slot_size, const size_t slot_count);
PAGE_DESTRUCT_DEFAULT(data_page, da)
struct data_page *da_construct_init(const size_t slot_size, const size_t slot_count);

bool da_is_full(const data_page *page);
bool da_is_empty(const data_page *page);

pageoff_t da_insert_data(struct data_page *page, const void *data, size_t size);
