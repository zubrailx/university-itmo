#include "database_meta.h"
#include "sections/database.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

char META_INFO[] = "version-DEV:zubrailx";

static void database_save(const Database *database, const char *meta) {
  DatabaseStored stored = database->ds;
  size_t total_size = sizeof(stored) + strlen(meta);
  assert(total_size <= stored.pos_empty && total_size <= stored.ds_first);
  // write meta inf
  FILE *file = database->file;
  rewind(file);
  fwrite(&stored, sizeof(stored), 1, file);
  fwrite(meta, strlen(meta), 1, file);
}

// needs file to read
static void database_read(Database *database) {
  DatabaseStored stored;
  rewind(database->file);
  assert(fread(&stored, sizeof(stored), 1, database->file));
  database->ds = stored;
}

Database database_create(const char *filename) {
  FILE *file = fopen(filename, "w+b");
  assert(file != NULL);

  fileoff_t offset = sizeof(DatabaseStored) + strlen(META_INFO);
  // init database entry to store in RAM
  Database database = {.file = file,
                       .name = strdup(filename),
                       .is_opened = true,
                       .ds = (DatabaseStored){
                           .is_corrupted = false,
                           .ds_first = offset,
                           .ds_last = offset,
                           .pos_empty = offset,
                       }};
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
  database_save(database, META_INFO);
  fclose(database->file);
  free(database->name);
  database->file = NULL;
  database->name = NULL;
  database->is_opened = false;
}
