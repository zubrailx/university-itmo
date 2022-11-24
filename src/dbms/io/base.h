#pragma once

#include <stdio.h>

#include <util/internals.h>

#include "page/base.h"

struct dbfile;

void page_load_size(struct pageoff_t *size, FILE *file, fileoff_t page_start);
void page_load(struct base_page *base, FILE *file, fileoff_t page_start);

// Load without type check
#define PAGE_DEFAULT_LOAD(type, prefix)                                                \
  void prefix##_load(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_LOAD_IMPL(type, prefix)                                           \
  void prefix##_load(type *page, FILE *file, fileoff_t page_start) {                   \
    page_load((struct base_page *)page, file, page_start);                             \
  }

// Create/alter/drop
#define PAGE_DEFAULT_CREATE(type, prefix)                                              \
  void prefix##_create(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_ALTER(type, prefix)                                               \
  void prefix##_alter(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_DROP(type, prefix)                                                \
  void prefix##_drop(FILE *file, fileoff_t page_start);

// Usage
PAGE_DEFAULT_CREATE(struct base_page, page)
PAGE_DEFAULT_ALTER(struct base_page, page)
PAGE_DEFAULT_DROP(struct base_page, page)

// Create/alter/drop implementations
#define PAGE_DEFAULT_CREATE_IMPL(type, prefix)                                         \
  void prefix##_create(type *page, FILE *file, fileoff_t page_start) {                 \
    page_create((base_page *)page, file, page_start);                                  \
  }

#define PAGE_DEFAULT_ALTER_IMPL(type, prefix)                                          \
  void prefix##_alter(type *page, FILE *file, fileoff_t page_start) {                  \
    page_alter((base_page *)page, file, page_start);                                   \
  }

#define PAGE_DEFAULT_DROP_IMPL(type, prefix)                                           \
  void prefix##_drop(FILE *file, fileoff_t page_start) { page_drop(file, page_start); }
