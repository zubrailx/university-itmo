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

void meta_load(dbmeta *meta, dbfile *db) {
  FILE *file = dbfile_file(db);
  rewind(file);
  assert(fread(meta, sizeof(dbmeta), 1, file));
}

void meta_alter(const dbmeta *meta, dbfile *dbfile) {
  FILE *file = dbfile_file(dbfile);
  rewind(file);
  fwrite(meta, sizeof(dbmeta), 1, file);
}

// Alias to meta_alter
void meta_create(const dbmeta *meta, dbfile *db) { meta_alter(meta, db); }
