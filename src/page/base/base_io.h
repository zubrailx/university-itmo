#pragma once

#include "../../util/types.h"

struct base_page;
struct dbfile;
struct page_size;

void page_load_size(struct page_size *size, struct dbfile *db, fileoff_t page_start);
void page_load(struct base_page *base, struct dbfile *db, fileoff_t page_start);

// Create/alter/drop
#define PAGE_DEFAULT_CREATE(type, alias)                                               \
  void alias##_create(type *page, struct dbfile *db, fileoff_t page_start);

#define PAGE_DEFAULT_ALTER(type, alias)                                                \
  void alias##_alter(type *page, struct dbfile *db, fileoff_t page_start);

#define PAGE_DEFAULT_DROP(type, alias)                                                 \
  void alias##_drop(struct dbfile *db, fileoff_t page_start);

// Usage
PAGE_DEFAULT_CREATE(struct base_page, page)
PAGE_DEFAULT_ALTER(struct base_page, page)
PAGE_DEFAULT_DROP(struct base_page, page)

// Create/alter/drop implementations
#define PAGE_DEFAULT_CREATE_IMPL(type, alias)                                          \
  void alias##_create(type *page, struct dbfile *db, fileoff_t page_start) {           \
    page_create((base_page *)page, db, page_start);                                    \
  }

#define PAGE_DEFAULT_ALTER_IMPL(type, alias)                                           \
  void alias##_alter(type *page, struct dbfile *db, fileoff_t page_start) {            \
    page_alter((base_page *)page, db, page_start);                                     \
  }

#define PAGE_DEFAULT_DROP_IMPL(type, alias)                                            \
  void alias##_drop(struct dbfile *db, fileoff_t page_start) {                         \
    page_drop(db, page_start);                                                         \
  }
