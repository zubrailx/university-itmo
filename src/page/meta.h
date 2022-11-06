#pragma once

#include "../dbtypes.h"
#include "../util.h"

/* Stored in file */
my_defstruct(database_meta);
struct database_meta {
  bool is_corrupted;
  struct {// database_page
    fileoff_t first;
    fileoff_t last;
  } dp;
  struct {
    fileoff_t first;
    fileoff_t last;
  } da;// data_page
  fileoff_t pos_empty;
};
