#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

sectoff_t DATABASE_SIZE = 1024;
char META_INFO[] = "version-DEV:zubrailx";

static inline size_t database_section_body_size(const DatabaseSection *dbs) {
  return dbs->header.base_section.size - sizeof(DatabaseHeader);
}

static DatabaseHeader create_database_header(sectoff_t sect_size) {
  DatabaseHeader dh;
  dh.base_section.type = TYPE_DATABASE;
  dh.base_section.size = sect_size;
  dh.off_table_last = sizeof(DatabaseHeader);
  dh.previous = SECTION_OFFSET_NULL;
  dh.next = SECTION_OFFSET_NULL;
  return dh;
}

// memory allocation
static DatabaseSection *database_section_malloc(const sectoff_t sect_size) {
  DatabaseSection *dbs = my_malloc(DatabaseSection);
  dbs->header = create_database_header(sect_size);
  dbs->body = malloc(database_section_body_size(dbs));
  return dbs;
}

// load + store in file
DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos) {
  FILE *file = database->file;
  DatabaseSection *created = database_section_malloc(DATABASE_SIZE);
  // calculate next position in file and write
  fileoff_t next_pos = database->pos_empty;
  fseek(file, next_pos, SEEK_SET);
  fwrite(&created->header, sizeof(created->header), 1, database->file);
  fwrite(created->body, database_section_body_size(created), 1, database->file);

  if (previous != SECTION_OFFSET_NULL) {
    previous->header.next = next_pos;
    fseek(file, previous_pos + offsetof(DatabaseHeader, next), SEEK_SET);
    fwrite(&previous->header.next, sizeof(previous->header.next), 1, file);
  }
  // fflush(file);
  database->pos_empty += created->header.base_section.size;
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
  // fflush(file);
}

// NULLABLE
DatabaseSection *database_section_load_next(Database *database,
                                            const DatabaseSection *current) {
  FILE *file = database->file;
  fileoff_t pos = current->header.next;
  if (pos == SECTION_OFFSET_NULL) {
    return NULL;
  }
  fseek(file, pos, SEEK_SET);
  return database_section_load(database, pos);
}

DatabaseSection *database_section_load(Database *database,
                                       const fileoff_t offset) {
  FILE *file = database->file;

  DatabaseSection *dbs = my_malloc(DatabaseSection);
  fseek(file, offset, SEEK_SET);
  assert(fread(&dbs->header, sizeof(dbs->header), 1, file));

  size_t body_size = database_section_body_size(dbs);
  dbs->body = malloc(body_size);
  assert(fread(dbs->body, body_size, 1, file));
  return dbs;
}

void database_section_unload(DatabaseSection **dbs_ptr) {
  DatabaseSection *dbs = *dbs_ptr;
  free(dbs->body);
  free(dbs);
  *dbs_ptr = NULL;
}

// DATABASE
static void database_store(const Database *database, const char *meta) {
  DatabaseStored stored =
      (DatabaseStored){.is_corrupted = database->is_corrupted,
                       .pos_empty = database->pos_empty,
                       .ds_first = database->ds_first,
                       .ds_last = database->ds_last};
  size_t total_size = sizeof(stored) + strlen(meta);
  assert(total_size <= stored.pos_empty && total_size <= stored.ds_first);
  // write meta inf
  FILE *file = database->file;
  rewind(file);
  fwrite(&stored, sizeof(stored), 1, file);
  fwrite(meta, strlen(meta), 1, file);
  fflush(file);
}

// needs file to read
static void database_read(Database *database) {
  DatabaseStored stored;
  rewind(database->file);
  assert(fread(&stored, sizeof(stored), 1, database->file));
  database->is_corrupted = stored.is_corrupted;
  database->ds_first = stored.ds_first;
  database->ds_last = stored.ds_last;
  database->pos_empty = stored.pos_empty;
}

Database database_create(const char *filename) {
  FILE *file = fopen(filename, "w+b");
  assert(file != NULL);

  fileoff_t offset = sizeof(DatabaseStored) + strlen(META_INFO);
  // init database entry to store in RAM
  Database database = {.file = file,
                       .name = strdup(filename),
                       .is_opened = true,
                       .is_corrupted = false,
                       .ds_first = offset,
                       .ds_last = offset,
                       .pos_empty = offset};
  // create first database section
  DatabaseSection *ds = database_section_create(&database, NULL, 0);
  database_section_unload(&ds);
  return database;
}

Database database_open(const char *filename) {
  FILE *file = fopen(filename, "r+b");
  assert(file != NULL);
  Database database;
  // init database entry to store in RAM
  database.is_opened = true;
  database.file = file;
  database.name = strdup(filename);
  database_read(&database);
  return database;
}

void database_close(Database *database) {
  database_store(database, META_INFO);
  fclose(database->file);
  free(database->name);
  database->file = NULL;
  database->name = NULL;
  database->is_opened = false;
}
