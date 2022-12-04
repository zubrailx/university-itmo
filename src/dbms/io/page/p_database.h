#pragma once

#include "index.h"
#include "p_base.h"
#include "sso.h"

/*
Structure: database_page
  |------------------|
  |HHHHHHH|T TTT  TTT| H - header
  |------------------| T - typles
  |                  | I - indexes to get typles because not fixed size
  |------------------|
  |              IIII|
  |------------------|

*/

// PAGE
typedef struct database_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t prev;
  pageoff_t index_start;// farthest to end of page index
  pageoff_t typle_end;  // after the last element
} database_header;

typedef struct database_page {
  struct database_header header;
  uint8_t body[];
} __attribute__((packed)) database_page;

// TYPLE
typedef struct dpt_header {
  bool is_present;
  size_t cols;
  struct page_sso sso;

  fileoff_t first; /* first and last page of the table */
  fileoff_t last;
  fileoff_t gappy_last; /* pointer to last elements in gappy */
} dpt_header;

typedef struct dpt_col_limits {
  bool is_null;
} dpt_col_limits;

typedef struct dpt_column {
  int8_t type; /* table_column_type */
  struct dpt_col_limits limits;
  struct page_sso sso;
} dpt_column;

typedef struct dp_typle {
  struct dpt_header header;
  struct dpt_column columns[];
} __attribute__((packed)) dp_typle;

// ITERATOR
typedef struct dp_typle_iter {
  struct database_page *page;
  pageoff_t icur;
  pageoff_t iend;// points after last element in list
} dp_typle_iter;

// Bodyoff to pageoff
INLINE_BODYOFF_TO_PAGEOFF(struct database_page, body, dp)
INLINE_PAGEOFF_TO_BODYOFF(struct database_page, body, dp)

// Constructors/destructors for database_page
PAGE_CONSTRUCT_DEFAULT(struct database_page, dp)
PAGE_DESTRUCT_DEFAULT(struct database_page, dp)
struct database_page *dp_construct_init(struct pageoff_t size, fileoff_t prev,
                                        fileoff_t next);

// Typle
size_t dp_space_left(const struct database_page *page);
size_t dp_typle_size(size_t columns);

pageoff_t dp_insert_typle(struct database_page *page, dp_typle *typle);
bool dp_drop_table(struct database_page *page, const pageoff_t pageoff);

dp_typle *dp_typle_locate(const struct database_page *page,
                          const pageoff_t idx_pageoff);

// Iterators
struct dp_typle_iter *dp_typle_iter_construct(struct database_page *page);
bool dp_typle_iter_next(struct dp_typle_iter *it);
struct dp_typle *dp_typle_iter_get(struct dp_typle_iter *it);
void dp_typle_iter_destruct(struct dp_typle_iter **it_ptr);
