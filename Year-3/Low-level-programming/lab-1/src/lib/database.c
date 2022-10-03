#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

sectoff_t DATABASE_SIZE = 1024;

static inline size_t database_section_body_size(const DatabaseSection *dbs) {
  return dbs->header->base_section.size - sizeof(DatabaseHeader);
}

static DatabaseHeader *database_header_malloc(sectoff_t sect_size) {
  DatabaseHeader *dh = my_malloc(DatabaseHeader);
  dh->base_section.type = TYPE_DATABASE;
  dh->base_section.size = sect_size;
  dh->next = SECTION_OFFSET_NULL;
  dh->previous = SECTION_OFFSET_NULL;
  dh->off_index_last = sizeof(DatabaseHeader);
  // TODO: idk nice offset stupid dogshit
  dh->off_typle_start = sect_size;
  return dh;
}

// memory allocation
static DatabaseSection *database_section_malloc(const sectoff_t sect_size) {
  DatabaseSection *dbs = my_malloc(DatabaseSection);
  dbs->header = database_header_malloc(sect_size);
  dbs->body = malloc(database_section_body_size(dbs));
  return dbs;
}

static void database_section_set_counter(DatabaseSection *section,
                                         Database *database) {
  section->header->base_section.number = database->ds.ds_counter;
  database->ds.ds_counter += 1;
}

// load + store in file
DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos) {
  FILE *file = database->file;
  DatabaseSection *created = database_section_malloc(DATABASE_SIZE);
  database_section_set_counter(created, database);
  // calculate next position in file and write
  fileoff_t next_pos = database->ds.pos_empty;
  fseek(file, next_pos, SEEK_SET);
  fwrite(created->header, sizeof(*created->header), 1, database->file);
  fwrite(created->body, database_section_body_size(created), 1, database->file);

  if (previous != SECTION_OFFSET_NULL) {
    previous->header->next = next_pos;
    fseek(file, previous_pos + offsetof(DatabaseHeader, next), SEEK_SET);
    fwrite(&previous->header->next, sizeof(previous->header->next), 1, file);
  }
  database->ds.pos_empty += created->header->base_section.size;
  return created;
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
  DatabaseSection *dbs = my_malloc(DatabaseSection);
  dbs->header = (DatabaseHeader *)section;
  dbs->body = section + sizeof(DatabaseHeader);
  return dbs;
}

void database_section_unload(DatabaseSection **dbs_ptr) {
  DatabaseSection *dbs = *dbs_ptr;
  free((void *)dbs->header);
  free(dbs);
  *dbs_ptr = NULL;
}
