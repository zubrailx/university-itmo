#pragma once

#include "dbtypes.h"
#include "util.h"

my_defstruct(po_ptr);
struct po_ptr {
  fileoff_t fileoff;
  pageoff_t offset;
};
