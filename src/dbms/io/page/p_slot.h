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

typedef struct slot_header {
  struct base_header base;
  size_t slot_size;
  size_t slot_count;
  pageoff_t slot_start;// slot number start
} slot_header;

typedef struct slot_page {
  struct slot_header header;
  uint8_t body[];
} slot_page;

// Bodyoff/pageoff
INLINE_BODYOFF_TO_PAGEOFF(struct slot_page, body, sp)
INLINE_PAGEOFF_TO_BODYOFF(struct slot_page, body, sp)

pageoff_t sp_size(size_t slot_size, size_t slot_count);

PAGE_CONSTRUCT_DEFAULT(struct slot_page, sp)
PAGE_DESTRUCT_DEFAULT(slot_page, sp)

slot_page *sp_construct_slot(size_t slot_size, size_t slot_count);
struct slot_page *sp_construct_slot_init(size_t slot_size, size_t slot_count);

bool sp_full(const slot_page *page);
bool sp_empty(const slot_page *page);

pageoff_t sp_insert_data(struct slot_page *page, const void *data, size_t size);
void sp_remove_data(struct slot_page *page, pageoff_t pageoff);
void *sp_select_data(struct slot_page *page, pageoff_t pageoff);
