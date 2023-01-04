#include "meta.h"

#include <assert.h>

// Meta is stored in file for proper loading
#include "meta/meta.h"

// preinit meta->da_len
void meta_load(struct meta *meta, FILE *file) {
  fseek(file, offsetof(struct meta, slot_len), SEEK_SET);
  // size is calculated based on da_len
  assert(fread(&meta->slot_len, sizeof(meta->slot_len), 1, file));
  size_t size = meta_size(meta);
  // read the whole
  rewind(file);
  assert(fread(meta, size, 1, file));
}

void meta_alter(const struct meta *meta, FILE *file) {
  rewind(file);
  fwrite(meta, meta_size(meta), 1, file);
}

// Alias to meta_alter
void meta_create(const struct meta *meta, FILE *file) { meta_alter(meta, file); }
