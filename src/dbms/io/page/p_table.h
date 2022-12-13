#pragma once

#include "column_types.h"
#include "p_base.h"
#include "sso.h"

// NOTE: currently without indexes
struct dp_tuple;
/*
Structure: table_page
  |------------------|
  |HHHHHHH|T TTT  TTT| H - header
  |------------------| T - tuples (structures for different types)
  |                  | (currently no indexes are present)
  |------------------| I - indexes of empty slots (slot_index)
  |         B      II| B - barier (max value for typle)
  |------------------|

*/
typedef struct slot_index {
  pageoff_t start;
} slot_index;

typedef struct table_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t prev;

  pageoff_t tuple_barrier;// exclusive
  //
  pageoff_t index_barrier;// inclusive
  pageoff_t index_start;  // if index_start == barrier then page is empty
} table_header;

typedef struct table_page {
  struct table_header header;
  uint8_t body[];
} table_page;

// Typle
#define TPT_COLUMN_BASE                                                                \
  { bool is_null; }

// pointer to the first element garanteed to be 0 padded
typedef struct tpt_column_base TPT_COLUMN_BASE tpt_column_base;

// NOTE: struct is packed inside tp_tuple that is why we need to add packed (else add
// columns not packed)
#define TPT_COLUMN(type, enum_name)                                                    \
  struct tpt_column_##enum_name {                                                      \
    struct TPT_COLUMN_BASE;                                                            \
    type entry;                                                                        \
  } __attribute__((packed));

// helpers
#define TPT_COLUMN_SIZE(enum_name) sizeof(struct tpt_column_##enum_name)
#define TPT_COL_TYPE(enum_name) tpt_column_##enum_name

// define columns
TPT_COLUMN(bool, COLUMN_TYPE_BOOL)
TPT_COLUMN(int32_t, COLUMN_TYPE_INT32)
TPT_COLUMN(double, COLUMN_TYPE_DOUBLE)
TPT_COLUMN(struct page_sso, COLUMN_TYPE_STRING)

typedef struct tpt_header {
  bool is_present;
} tpt_header;

typedef struct tp_tuple {
  struct tpt_header header;
  struct tpt_column_base columns[];// just the pointer to the first entry start (packed)
} tp_tuple;

// To create tp_tuple from dto
typedef struct tpt_col_info {
  size_t start;
} tpt_col_info;

// Iterators
typedef struct tp_tuple_iter {
  struct table_page *page;
  pageoff_t tcur;
  pageoff_t tend;
  size_t tuple_size;
} tp_tuple_iter;

INLINE_BODYOFF_TO_PAGEOFF(struct table_page, body, tp)
INLINE_PAGEOFF_TO_BODYOFF(struct table_page, body, tp)

// Constructors/destructors
PAGE_CONSTRUCT_DEFAULT(struct table_page, tp)
PAGE_DESTRUCT_DEFAULT(struct table_page, tp)
struct table_page *tp_construct_init(struct pageoff_t size, fileoff_t prev,
                                     fileoff_t next, const struct dp_tuple *tuple);

pageoff_t tp_get_min_size(const struct dp_tuple *tuple);
size_t tp_get_tuple_size(const struct dp_tuple *tuple);
tpt_col_info *tp_construct_col_info_arr(const struct dp_tuple *tuple);

bool tp_is_full(const struct table_page *page);
bool tp_is_empty(const struct table_page *page);

pageoff_t tp_insert_row(struct table_page *page, const tp_tuple *tuple,
                        size_t tuple_size);
void tp_update_row(struct table_page *page, const tp_tuple *tpt_new, size_t tuple_size,
                   const pageoff_t start);
void tp_update_row_ptr(struct table_page *page, const tp_tuple *tpt_new,
                       size_t tuple_size, tp_tuple *tpt_old);
void tp_remove_row(struct table_page *page, const pageoff_t start);
void tp_remove_row_ptr(struct table_page *page, tp_tuple *tpt_old);

// Iterators
struct tp_tuple_iter *tp_tuple_iter_construct(struct table_page *page,
                                              size_t typle_size);
bool tp_tuple_iter_next(struct tp_tuple_iter *it);
struct tp_tuple *tp_tuple_iter_get(struct tp_tuple_iter *it);
void tp_tuple_iter_destruct(struct tp_tuple_iter **it_ptr);
