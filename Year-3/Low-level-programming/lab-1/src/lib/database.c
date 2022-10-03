#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

sectoff_t DATABASE_SECTION_SIZE = 1024;

static inline size_t database_section_body_size(const DatabaseSection *dbs) {
  return dbs->header->base_section.size - sizeof(DatabaseHeader);
}

inline sectoff_t database_section_get_body_offset(sectoff_t sectoff) {
  return sectoff - sizeof(DatabaseHeader);
}
inline sectoff_t database_section_get_sectoff(sectoff_t bodyoff) {
  return bodyoff + sizeof(DatabaseHeader);
}

static DatabaseSection *database_section_convert_section(void *section) {
  DatabaseSection *dbs = my_malloc(DatabaseSection);
  // offset for header equals 0
  dbs->header = (DatabaseHeader *)section;
  dbs->body = section + sizeof(DatabaseHeader);
  return dbs;
}

// memory allocation
static DatabaseSection *database_section_malloc(const sectoff_t sect_size) {
  void *section = section_malloc(sect_size);
  return database_section_convert_section(section);
}

static void database_section_init(DatabaseSection *ds) {
  DatabaseHeader *dh = ds->header;
  // Init database section
  dh->base_section.type = TYPE_DATABASE;
  dh->base_section.size = DATABASE_SECTION_SIZE;
  dh->next = SECTION_OFFSET_NULL;
  dh->previous = SECTION_OFFSET_NULL;
  dh->off_index_last = sizeof(DatabaseHeader);
  // TODO: idk nice offset stupid dogshit
  dh->off_typle_start = DATABASE_SECTION_SIZE;
}

// load + store in file
DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos) {
  FILE *file = database->file;
  DatabaseSection *ds = database_section_malloc(DATABASE_SECTION_SIZE);
  database_section_init(ds);
  // calculate next position in file and write
  fileoff_t next_pos = database->ds.pos_empty;
  fseek(file, next_pos, SEEK_SET);
  fwrite(ds->header, sizeof(*ds->header), 1, database->file);
  fwrite(ds->body, database_section_body_size(ds), 1, database->file);

  if (previous != SECTION_OFFSET_NULL) {
    previous->header->next = next_pos;
    fseek(file, previous_pos + offsetof(DatabaseHeader, next), SEEK_SET);
    fwrite(&previous->header->next, sizeof(previous->header->next), 1, file);
  }
  database->ds.pos_empty += ds->header->base_section.size;
  return ds;
}

void database_section_delete(Database *database, fileoff_t pos) {
  FILE *file = database->file;
  DatabaseHeader current;
  // set current section type dumped
  fseek(file, pos, SEEK_SET);
  assert(fread(&current, sizeof(current), 1, file));
  current.base_section.type = TYPE_DUMPED;
  fwrite(&current, sizeof(current), 1, file);
  // update relatives
  if (current.next != SECTION_OFFSET_NULL) {
    fseek(file, current.next, SEEK_SET);
    fwrite(&current.previous, sizeof(current.previous), 1, file);
  }
  if (current.previous != SECTION_OFFSET_NULL) {
    fseek(file, current.previous, SEEK_SET);
    fwrite(&current.next, sizeof(current.next), 1, file);
  }
}

// NULLABLE
DatabaseSection *database_section_load_next(Database *database,
                                            const DatabaseSection *current) {
  FILE *file = database->file;
  fileoff_t pos = current->header->next;
  if (pos == SECTION_OFFSET_NULL) {
    return NULL;
  }
  fseek(file, pos, SEEK_SET);
  return database_section_load(database, pos);
}

DatabaseSection *database_section_load(Database *database,
                                       const fileoff_t offset) {
  void *section = section_load(database, offset);
  return database_section_convert_section(section);
}

void database_section_unload(DatabaseSection **dbs_ptr) {
  // section is allocated inseverable
  DatabaseSection *dbs = *dbs_ptr;
  void *section_start = dbs->header;
  section_unload((void **) &section_start);
  free(*dbs_ptr);
  *dbs_ptr = NULL;
}
