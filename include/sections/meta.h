#pragma once

#include "util.h"
#include "dbtypes.h"

/* Stored in file */
my_defstruct(DatabaseMeta);
struct DatabaseMeta {
  bool is_corrupted;
  fileoff_t ds_first; // first DatabaseSection
  fileoff_t ds_last;
  fileoff_t da_last; // first DataSection
  fileoff_t da_first;
  fileoff_t pos_empty;
};
