#include "meta_io.h"

#include <assert.h>

#include "meta.h"

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
