#pragma once

#include "base.h"
#include "sso.h"

// NOTE: currently without indexes

/*
Structure: table_page
  |------------------|
  |HHHHHHH|T TTT  TTT| H - header
  |------------------| T - typles (structures for different types)
  |                  | (currently no indexes are present)
  |------------------|
  |                  |
  |------------------|

*/
typedef struct table_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t prev;
  pageoff_t typle_end;
} table_header;

typedef struct table_page {
  struct table_header header;
  uint8_t body[];
} __attribute__((packed)) table_page;

INLINE_BODYOFF_TO_PAGEOFF(table_page, body, tp)
INLINE_PAGEOFF_TO_BODYOFF(table_page, body, tp)

// columns types
#define _TPT_ENTRY_BASE                                                                \
  {                                                                                    \
    bool is_present;                                                                   \
    bool is_null;                                                                      \
  }

typedef struct tpt_entry_base _TPT_ENTRY_BASE tpt_entry_base;

#define TPT_COLUMN_ENTRY(type, suffix)                                                 \
  typedef struct tpt_entry_##suffix {                                                  \
    struct _TPT_ENTRY_BASE;                                                            \
    char entry[sizeof(type)];                                                          \
  } tpt_entry_##suffix;

TPT_COLUMN_ENTRY(double, double)
TPT_COLUMN_ENTRY(uint32_t, uint32_t)
TPT_COLUMN_ENTRY(bool, bool)
TPT_COLUMN_ENTRY(struct page_sso, sso)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT(table_page, tp)
PAGE_DESTRUCT_DEFAULT(table_page, tp)
struct table_page *tp_construct_init(struct pageoff_t size, fileoff_t prev,
                                     fileoff_t next);

// TODO: ITERATORS(for tpt_entry_base)
