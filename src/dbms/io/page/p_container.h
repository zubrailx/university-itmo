#pragma once

#include "p_base.h"

/*
Structure: container_page
  |------------------|
  |HHHHHHH|TTTT      | H - header
  |------------------| T - entries
  |                  |
  |------------------|
  |                  |
  |------------------|

*/

typedef struct container_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t prev;

  pageoff_t end;
} container_header;

typedef struct page_container {
  struct container_header header;
  uint8_t body[];
} __attribute__((packed)) page_container;

typedef struct page_entry {
  fileoff_t start;
} page_entry;

PAGE_CONSTRUCT_DEFAULT(page_container, container)
PAGE_DESTRUCT_DEFAULT(page_container, container)

page_container *container_construct_init(pageoff_t size, fileoff_t prev,
                                         fileoff_t next);

bool container_push(struct page_container *page, const struct page_entry *entry);
struct page_entry *container_pop(struct page_container *page);
struct page_entry *container_top(const struct page_container *page);
bool container_empty(const struct page_container *page);

inline page_entry page_entry_construct(fileoff_t fileoff) {
  return (page_entry){.start = fileoff};
}
