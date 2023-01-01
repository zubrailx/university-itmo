#pragma once

#include <util/internals.h>

typedef struct po_ptr {
  fileoff_t fileoff;
  pageoff_t offset;
} po_ptr;
