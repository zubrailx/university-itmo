#pragma once

#include <stdio.h>

#include <dbms/internals/po_ptr.h>

#include "page/p_base.h"

struct dbfile;

void page_select_size(struct pageoff_t *size, FILE *file, fileoff_t page_start);
void page_select(struct base_page *base, FILE *file, fileoff_t page_start);

// Load without type check
#define PAGE_DEFAULT_SELECT(type, prefix)                                              \
  void prefix##_select(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_SELECT_IMPL(type, prefix)                                         \
  void prefix##_select(type *page, FILE *file, fileoff_t page_start) {                 \
    page_select((struct base_page *)page, file, page_start);                           \
  }

// Create/alter/drop
#define PAGE_DEFAULT_CREATE(type, prefix)                                              \
  void prefix##_create(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_ALTER(type, prefix)                                               \
  void prefix##_alter(type *page, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_DROP(type, prefix)                                                \
  void prefix##_drop(FILE *file, fileoff_t page_start);

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

// Construct + select
#define PAGE_DEFAULT_CONSTRUCT_SELECT(type, prefix)                                    \
  type *prefix##_construct_select(FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_CONSTRUCT_SELECT_IMPL(type, prefix)                               \
  type *prefix##_construct_select(FILE *file, fileoff_t page_start) {                  \
    pageoff_t size;                                                                    \
    page_select_size(&size, file, page_start);                                         \
    type *page = prefix##_construct(size);                                             \
    prefix##_select(page, file, page_start);                                           \
    return page;                                                                       \
  }

// Destruct + unload
#define PAGE_DEFAULT_ALTER_DESTRUCT(type, prefix)                                      \
  void prefix##_alter_destruct(type **page_ptr, FILE *file, fileoff_t page_start);

#define PAGE_DEFAULT_ALTER_DESTRUCT_IMPL(type, prefix)                                 \
  void prefix##_alter_destruct(type **page_ptr, FILE *file, fileoff_t page_start) {    \
    prefix##_alter(*page_ptr, file, page_start);                                       \
    prefix##_destruct(page_ptr);                                                       \
  }

// Usage
PAGE_DEFAULT_CREATE(struct base_page, page)
PAGE_DEFAULT_ALTER(struct base_page, page)
PAGE_DEFAULT_DROP(struct base_page, page)
