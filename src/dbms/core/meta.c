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
