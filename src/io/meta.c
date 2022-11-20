#include "meta.h"

#include <assert.h>

#include "../dbms/meta.h"

void meta_load(dbmeta *meta, FILE *file) {
  rewind(file);
  assert(fread(meta, sizeof(dbmeta), 1, file));
}

void meta_alter(const dbmeta *meta, FILE *file) {
  rewind(file);
  fwrite(meta, sizeof(dbmeta), 1, file);
}

// Alias to meta_alter
void meta_create(const dbmeta *meta, FILE *file) { meta_alter(meta, file); }
