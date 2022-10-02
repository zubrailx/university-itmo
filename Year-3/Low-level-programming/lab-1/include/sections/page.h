#pragma once

#include <stdbool.h>

#include "base.h"

// PAGE
typedef struct PageSection PageSection;

// PageHeader
typedef struct PageHeader {
  BaseSection base_section;
  fileoff_t page_previous;
  fileoff_t page_next;
  /* non-inclusive (off_typle_info_end - off_typle_info + 1) == empty_size*/
  uint32_t off_typle_info_end;
  uint32_t off_typle_end;
} PageHeader;

// PageBody
typedef struct PageFlags {
  /* Is entry is present or deleted */
  bool is_valid;
} PageFlags;

typedef struct PageLinePointer {
  /* Offset start inclusive, which points on start */
  uint32_t off_start_inc;
  /* Offset end non inclusive, which points over end */
  uint32_t off_end_noninc;
  PageFlags flags;
} PageLinePointer;

// Page
struct PageSection {
  PageHeader header;
  void *body;
};

// DATA
typedef struct PageDataHeader {
  BaseSection base_section;
  fileoff_t next;
} PageDataHeader;

typedef struct PageDataSection {
  PageDataHeader header;
  void *body;
} Data;
