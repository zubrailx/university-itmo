#include "sections/base.h"

#include <assert.h>
#include <malloc.h>

void *section_malloc(const sectoff_t sect_size) { return malloc(sect_size); }

void *section_load(Database *database, const fileoff_t offset) {
  FILE *file = database->file;
  BaseSection base;
  fseek(file, offset, SEEK_SET);
  assert(fread(&base, sizeof(base), 1, file));
  void *section = section_malloc(base.size);
  assert(fread(section, base.size, 1, file));
  return section;
}

void section_unload(void **base_ptr) {
  free(*base_ptr);
  *base_ptr = NULL;
}
