#include "meta.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "meta_io.h"

dbmeta *meta_construct() {
  dbmeta *meta = my_malloc(dbmeta);
  return meta;
}

void meta_destruct(dbmeta **meta_ptr) {
  free(*meta_ptr);
  *meta_ptr = NULL;
}

// Prepare meta for creation (init with pointers)
void meta_init_pages(dbmeta *meta, fileoff_t dp_first, fileoff_t da_first) {
  meta->pos_empty = sizeof(dbmeta);
  meta->dp.first = dp_first;
  meta->dp.last = dp_first;
  meta->da.first = da_first;
  meta->da.last = da_first;
}

