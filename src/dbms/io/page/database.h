#pragma once

#include "base.h"
#include "index.h"
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
  bodyoff_t index_start;// farthest to end of page index
  bodyoff_t typle_end;  // after the last element
} database_header;

typedef struct database_page {
  struct database_header header;
  uint8_t body[];
} __attribute__((packed)) database_page;

// Bodyoff to pageoff
BODYOFF_TO_PAGEOFF(database_header, dp)
PAGEOFF_TO_BODYOFF(database_header, dp)

struct database_page *dp_construct(struct pageoff_t size);
void dp_destruct(struct database_page **page_ptr);

// TYPLE
typedef struct dpt_header {
  fileoff_t fileoff;// first page of table
  size_t cols;
  struct page_sso sso;
} dpt_header;

enum table_column_type {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
  COLUMN_TYPE_UNKNOWN
};

typedef struct dpt_col_limits {
  bool is_null;
  bool is_unique;
} dpt_col_limits;

typedef struct dpt_column {
  /* TableColumnType */
  int8_t type;
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

struct dp_typle_iter dp_typle_iterator(struct database_page *page);
bool dp_typle_iter_next(struct dp_typle_iter *it);
struct dp_typle *dp_typle_iter_get(struct dp_typle_iter *it);
