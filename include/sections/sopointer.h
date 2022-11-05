#pragma once

#include "dbtypes.h"
#include "util.h"

my_defstruct(SOPointer);
struct SOPointer {
  fileoff_t fileoff;
  sectoff_t offset;
};
