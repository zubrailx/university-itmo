#include "meta.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <util/define.h>

dbmeta *meta_construct() {
  dbmeta *meta = my_malloc(dbmeta);
  *meta = (dbmeta){.pos_empty = get_fileoff_t(sizeof(dbmeta))};
  return meta;
}

void meta_destruct(dbmeta **meta_ptr) {
  free(*meta_ptr);
  *meta_ptr = NULL;
}

// Prepare meta for creation (init with pointers)
void meta_init_pages(dbmeta *meta, fileoff_t dp_first, fileoff_t da_first) {
  meta->dp.first = dp_first;
  meta->dp.last = dp_first;
  meta->da.first = da_first;
  meta->da.last = da_first;
}

fileoff_t meta_get_next_pos(const dbmeta *meta) { return meta->pos_empty; }
void meta_set_dp_last_pos(dbmeta *meta, fileoff_t dp_last) { meta->dp.last = dp_last; }
void meta_set_da_last_pos(dbmeta *meta, fileoff_t da_last) { meta->da.last = da_last; }
