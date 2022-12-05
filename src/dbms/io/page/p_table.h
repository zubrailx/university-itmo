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
  |------------------| I - empty slots
  |         B      II| B - barier (max value for typle)
  |------------------|

*/
typedef struct empty_slot {
  pageoff_t start;
} empty_slot;

typedef struct table_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t prev;

  pageoff_t tuple_barrier; // exclusive
  pageoff_t slot_barrier; // inclusive
  pageoff_t empty_start;// if empty_start == barrier then page is empty
} table_header;

typedef struct table_page {
  struct table_header header;
  uint8_t body[];
} __attribute__((packed)) table_page;

// Typle
#define _TPT_COLUMN_BASE                                                               \
  { bool is_null; }

typedef struct tpt_column_base _TPT_COLUMN_BASE tpt_column_base;

#define TPT_COLUMN(type, enum)                                                         \
  typedef struct tpt_column_##type {                                                   \
    struct _TPT_COLUMN_BASE;                                                           \
    char entry[sizeof(type)];                                                          \
  } tpt_entry_##type;                                                                  \
                                                                                       \
  inline size_t tpt_column_##enum##_size() { return sizeof(struct tpt_column_##type); }

#define EXTERN_INLINE_TPT_COLUMN(type, enum) extern size_t tpt_column_##enum##_size();

#define _TPT_COLUMN_SIZE(enum) tpt_column_##enum##_size()

TPT_COLUMN(double, COLUMN_TYPE_DOUBLE)
TPT_COLUMN(int32_t, COLUMN_TYPE_INT32)
TPT_COLUMN(bool, COLUMN_TYPE_BOOL)
TPT_COLUMN(page_sso, COLUMN_TYPE_STRING)

typedef struct tpt_header {
  bool is_present;
} tpt_header;

typedef struct tp_tuple {
  struct tpt_header header;
  struct tpt_column_base columns[];// just the pointer to the first entry start
} __attribute__((packed)) tp_tuple;

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
struct table_page *tp_construct_init(const struct pageoff_t size, const fileoff_t prev,
                                     const fileoff_t next,
                                     const struct dp_tuple *tuple);

pageoff_t tp_get_min_size(const struct dp_tuple *tuple);
size_t tp_get_tuple_size(const struct dp_tuple *tuple);
// Iterators
struct tp_tuple_iter *tp_tuple_iter_construct(struct table_page *page,
                                              size_t typle_size);
bool tp_tuple_iter_next(struct tp_tuple_iter *it);
struct tp_tuple *tp_tuple_iter_get(struct tp_tuple_iter *it);
void tp_tuple_iter_destruct(struct tp_tuple_iter **it_ptr);
