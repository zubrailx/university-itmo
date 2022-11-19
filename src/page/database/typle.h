#pragma once

#include "../../util/sso.h"
#include "../../util/types.h"

// FILE
my_defstruct(dpt_header);
struct dpt_header {
  fileoff_t fileoff;// first page of table
  size_t cols;
  struct page_sso sso;
};

enum table_column_types {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
};

my_defstruct(dpt_col_limits);
struct dpt_col_limits {
  bool is_null;
  bool is_unique;
};

my_defstruct(dpt_column);
struct dpt_column {
  /* TableColumnType */
  int8_t type;
  struct dpt_col_limits limits;
  struct page_sso sso;
};

my_defstruct(dp_typle);
struct dp_typle {
  struct dpt_header header;
  struct dpt_column columns[];
} __attribute__((packed));

// Wrappers to contain proper address of pages that are loaded in ram)
// NOTE: will be replaced after pagepool implemented

// my_defstruct(dpt_column_wr);
// struct dpt_column_wr {
//   void *addr;      // address of string in ram (if it is not sso)
//   pageoff_t offset;// offset of this ram page
//   bool is_staged;
//   struct dpt_column column;
// };

// my_defstruct(dpt_header_wr);
// struct dpt_header_wr {
//   void *addr;
//   pageoff_t offset;
//   bool is_staged;
//   struct dpt_header header;
// };

// my_defstruct(dpt_typle_wr);
// struct dpt_typle_wr {
//   struct dpt_header_wr header;
//   struct dpt_column_wr columns[];
// } __attribute__((packed));
