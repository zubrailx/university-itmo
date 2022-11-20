#pragma once

#include "../../page/base.h"
#include "../../page/internals.h"

struct dbfile;

void page_load_size(struct pageoff_t *size, struct dbfile *db, fileoff_t page_start);
void page_load(struct base_page *base, struct dbfile *db, fileoff_t page_start);

// Create/alter/drop
#define PAGE_DEFAULT_CREATE(type, prefix)                                              \
  void prefix##_create(type *page, struct dbfile *db, fileoff_t page_start);

#define PAGE_DEFAULT_ALTER(type, prefix)                                               \
  void prefix##_alter(type *page, struct dbfile *db, fileoff_t page_start);

#define PAGE_DEFAULT_DROP(type, prefix)                                                \
  void prefix##_drop(struct dbfile *db, fileoff_t page_start);

// Usage
PAGE_DEFAULT_CREATE(struct base_page, page)
PAGE_DEFAULT_ALTER(struct base_page, page)
PAGE_DEFAULT_DROP(struct base_page, page)

// Create/alter/drop implementations
#define PAGE_DEFAULT_CREATE_IMPL(type, prefix)                                         \
  void prefix##_create(type *page, struct dbfile *db, fileoff_t page_start) {          \
    page_create((base_page *)page, db, page_start);                                    \
  }

#define PAGE_DEFAULT_ALTER_IMPL(type, prefix)                                          \
  void prefix##_alter(type *page, struct dbfile *db, fileoff_t page_start) {           \
    page_alter((base_page *)page, db, page_start);                                     \
  }

#define PAGE_DEFAULT_DROP_IMPL(type, prefix)                                           \
  void prefix##_drop(struct dbfile *db, fileoff_t page_start) {                        \
    page_drop(db, page_start);                                                         \
  }
