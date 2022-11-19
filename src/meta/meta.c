#include "meta_io.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "../database.h"
#include "meta.h"
#include "meta_io.h"

database_meta *meta_construct() {
  database_meta *meta = malloc(sizeof(database_meta));
  return meta;
}

void meta_destruct(database_meta **meta_ptr) {
  free(*meta_ptr);
  *meta_ptr = NULL;
}

// Prepare meta for creation (init with pointers)
void meta_prepare_create(database_meta *meta, fileoff_t dp_first, fileoff_t da_first) {
  meta->pos_empty = sizeof(database_meta);
  meta->dp.first = dp_first;
  meta->dp.last = dp_first;
  meta->da.first = da_first;
  meta->da.last = da_first;
}

void meta_select(database_meta *meta, const database *db) {
  FILE *file = database_get_file(db);
  rewind(file);
  assert(!fread(meta, sizeof(database_meta), 1, file));
}

void meta_alter(const database_meta *meta, database *db) {
  FILE *file = database_get_file(db);
  rewind(file);
  fwrite(meta, sizeof(database_meta), 1, file);
}

// Alias to meta_alter
void meta_create(const database_meta *meta, database *db) { meta_alter(meta, db); }
