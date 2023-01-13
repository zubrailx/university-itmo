#pragma once

#include <dbms/internals/page_loc.h>

typedef struct po_ptr {
  fileoff_t fileoff;
  pageoff_t offset;
} po_ptr;
