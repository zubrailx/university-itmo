#include "base.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

static base_page DUMPED = {.type = PAGE_DUMPED};

base_page *page_malloc(const pageoff_t sect_size) {
  return (base_page *)malloc(sect_size);
}

base_page *page_load(const database *database, const fileoff_t offset) {
  FILE *file = database->file;
  base_page base;
  fseek(file, (long)offset, SEEK_SET);
  assert(fread(&base, sizeof(base), 1, file));
  void *section = page_malloc(base.size);
  fseek(file, (long)offset, SEEK_SET);
  assert(fread(section, base.size, 1, file));
  return section;
}

base_page *page_load_check_type(const database *database, const fileoff_t fileoff,
                                int8_t type) {
  base_page *sect = (base_page *)page_load(database, fileoff);
  if (sect->type != PAGE_DATABASE) {
    page_unload((base_page **)&sect);
  }
  return sect;
}

base_page *page_header_load(const database *database, const fileoff_t fileoff,
                            size_t size) {
  FILE *file = database->file;
  fseek(file, (long)fileoff, SEEK_SET);
  base_page *base = malloc(size);
  assert(fread(&base, size, 1, file));
  return base;
}

void page_unload(base_page **base_ptr) {
  free(*base_ptr);
  *base_ptr = NULL;
}

// Returns section pos
fileoff_t page_create(database *database, const base_page *sect) {
  FILE *file = database->file;
  assert(file != NULL);

  fileoff_t next_pos = database->dst->pos_empty;
  database->dst->pos_empty += sect->size;

  fseek(file, (long)next_pos, SEEK_SET);
  fwrite(sect, sect->size, 1, file);
  return next_pos;
}

bool page_alter(database *database, const fileoff_t fileoff, const base_page *sect) {
  FILE *file = database->file;
  assert(file != NULL);

  fseek(file, (long)fileoff, SEEK_SET);
  return fwrite(sect, sect->size, 1, file);
}

bool page_alter_pageoff(database *database, const fileoff_t fileoff,
                        const pageoff_t offset, const void *data, const size_t size) {
  FILE *file = database->file;
  assert(file != NULL);

  fseek(file, (long)fileoff + (long)offset, SEEK_SET);
  return fwrite(data, size, 1, file);
}

bool page_alter_pageoff_sync(database *database, const fileoff_t fileoff,
                             const pageoff_t offset, base_page *sect, const void *data,
                             const size_t size) {
  memcpy((void *)sect + offset, data, size);
  return page_alter_pageoff(database, fileoff, offset, data, size);
}

bool page_drop(database *database, const fileoff_t fileoff) {
  FILE *file = database->file;
  assert(file != NULL);

  fseek(file, (long)fileoff, SEEK_SET);
  return fwrite(&DUMPED.type, sizeof(DUMPED.type), 1, file);
}
// Sync means that loaded structure and files are both overwritten, RAM is not
// deleted but header section type is set to dumped
bool page_drop_sync(database *database, const fileoff_t fileoff, base_page *sect) {
  sect->type = PAGE_DUMPED;
  return page_drop(database, fileoff);
}
