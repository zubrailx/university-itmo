#pragma once

#include "index.h"
#include "p_base.h"
#include "sso.h"

/*
Structure: database_page
  |------------------|
  |HHHHHHH|T TTT  TTT| H - header
  |------------------| T - tuples
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
  pageoff_t tuple_end;  // after the last element
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

  fileoff_t tp_first; /* first and last page of the table */
  fileoff_t tp_last;

  fileoff_t td_last; /* pointer to last elements in gappy */
} dpt_header;

typedef struct dpt_col_limits {
  bool is_null;
} dpt_col_limits;

typedef struct dpt_column {
  int8_t type; /* table_column_type */
  struct dpt_col_limits limits;
  struct page_sso sso;
} dpt_column;

typedef struct dp_tuple {
  struct dpt_header header;
  struct dpt_column columns[];
} __attribute__((packed)) dp_tuple;

// ITERATOR
typedef struct dp_tuple_iter {
  struct database_page *page;
  pageoff_t icur;
  pageoff_t iend;// points after last element in list
} dp_tuple_iter;

// Bodyoff to pageoff
INLINE_BODYOFF_TO_PAGEOFF(struct database_page, body, dp)
INLINE_PAGEOFF_TO_BODYOFF(struct database_page, body, dp)

// Constructors/destructors for database_page
PAGE_CONSTRUCT_DEFAULT(struct database_page, dp)
PAGE_DESTRUCT_DEFAULT(struct database_page, dp)
struct database_page *dp_construct_init(struct pageoff_t size, fileoff_t prev,
                                        fileoff_t next);

// Tuple
size_t dp_space_left(const struct database_page *page);
size_t dp_tuple_size(size_t columns);

dp_tuple *dp_tuple_locate(const struct database_page *page,
                          const pageoff_t idx_pageoff);

// insert and delete tuple in page
pageoff_t dp_insert_table(struct database_page *page, dp_tuple *typle);
bool dp_drop_table(struct database_page *page, const pageoff_t pageoff);

// Iterators
struct dp_tuple_iter *dp_tuple_iter_construct(struct database_page *page);
bool dp_tuple_iter_next(struct dp_tuple_iter *it);
struct dp_tuple *dp_tuple_iter_get(struct dp_tuple_iter *it);
void dp_tuple_iter_destruct(struct dp_tuple_iter **it_ptr);
