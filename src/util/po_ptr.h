#pragma once

#include "types.h"
#include "define.h"

my_defstruct(po_ptr);
struct po_ptr {
  fileoff_t fileoff;
  pageoff_t offset;
};
