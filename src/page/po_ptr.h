#pragma once

#include "../util/define.h"
#include "internals.h"

my_defstruct(po_ptr);
struct po_ptr {
  fileoff_t fileoff;
  pageoff_t offset;
};
