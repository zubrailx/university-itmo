#pragma once

#include <stdbool.h>

#include "../dbtypes.h"
#include "../util.h"

/* Stored in file */
my_defstruct(database_meta);
struct database_meta {
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

database_meta *meta_construct();
void meta_destruct(database_meta **meta_ptr);
void meta_prepare_create(database_meta *meta, fileoff_t dp_first, fileoff_t da_first);
