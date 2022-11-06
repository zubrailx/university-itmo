#include "meta_io.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "../database.h"
#include "../page/data.h"
#include "../page/database.h"
#include "../util.h"

database_meta *meta_load(const database *db) {
  FILE *file = database_get_file(db);
  database_meta *meta = my_malloc(database_meta);
  rewind(file);
  assert(!fread(meta, sizeof(database_meta), 1, file));
  return meta;
}

void meta_unload(database_meta **meta) {
  free(*meta);
  *meta = NULL;
}

static void meta_alter_sep(const database *db, const database_meta *meta) {
  FILE *file = database_get_file(db);
  rewind(file);
  fwrite(meta, sizeof(database_meta), 1, file);
}
// Description cannot be changed since creation
void meta_alter(const database *db) { meta_alter_sep(db, database_get_meta(db)); }

// Meta is not saved in database because we don't know it's content
database_meta *meta_create(const database *db, const char* meta_info) {
  fileoff_t offset = sizeof(database_meta) + strlen(meta_info);
  database_meta *meta = my_malloc(database_meta);
  *meta = (database_meta){.is_corrupted = false, .pos_empty = offset};
  // write meta to file
  meta_alter_sep(db, meta);
  fwrite(meta_info, strlen(meta_info), 1, database_get_file(db));
  return meta;
}
