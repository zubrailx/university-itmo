#pragma once

#include "p_base.h"

/*
Structure: container_page
  |------------------|
  |HHHHHHH|          | H - header
  |------------------| T - entries
  |                  |
  |------------------|
  |             TTTTT|
  |------------------|

*/

typedef struct container_header {
  struct base_header base;
  fileoff_t prev;

  pageoff_t start;// points to the first element ->[][]
} container_header;

typedef struct page_container {
  struct container_header header;
  uint8_t body[];
} __attribute__((packed)) page_container;

typedef struct page_entry {
  fileoff_t start;
  pageoff_t size;// size of page (because some pages can be too small)
} page_entry;

typedef struct cp_entry_iter {
  struct page_container *page;
  pageoff_t cur;
  pageoff_t end;// points after last element in list
} cp_entry_iter;

PAGE_CONSTRUCT_DEFAULT(struct page_container, container)
PAGE_DESTRUCT_DEFAULT(struct page_container, container)
page_container *container_construct_init(pageoff_t size, fileoff_t prev);

bool container_push(struct page_container *page, const struct page_entry *entry);
struct page_entry *container_pop(struct page_container *page);
struct page_entry *container_top(const struct page_container *page);
bool container_full(const struct page_container *page);
bool container_empty(const struct page_container *page);

inline page_entry page_entry_construct(fileoff_t fileoff) {
  return (page_entry){.start = fileoff};
}

// Iterator
struct cp_entry_iter *cp_entry_iter_construct(struct page_container *page);
void cp_entry_iter_destruct(struct cp_entry_iter **it_ptr);
bool cp_entry_iter_next(struct cp_entry_iter *it);
struct page_entry *cp_entry_iter_get(struct cp_entry_iter *it);
