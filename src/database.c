#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "database.h"
#include "sect/data.h"
#include "sect/dbase.h"

const char META_INFO[] = "version-DEV:zubrailx";

// needs file to read
static void database_load(Database *database) {
  DatabaseMeta stored;
  rewind(database->file);
  assert(fread(&stored, sizeof(stored), 1, database->file));
  database->dst = stored;
}

Database database_create(const char *filename) {
  FILE *file = fopen(filename, "w+b");
  assert(file != NULL);

  fileoff_t offset = sizeof(DatabaseMeta) + strlen(META_INFO);
  // init database entry to store in RAM
  Database database = {.file = file,
                       .name = strdup(filename),
                       .is_opened = true,
                       .dst = (DatabaseMeta){
                           .is_corrupted = false,
                           .pos_empty = offset,
                       }};
  // create first sections database section
  DatabaseSectionWr dswr = ds_create(&database, NULL, 0);
  database.dst.ds_first = dswr.fileoff;
  database.dst.ds_last = dswr.fileoff;
  DataSectionWr dawr = da_create(&database);
  database.dst.da_first = dswr.fileoff;
  database.dst.da_last = dswr.fileoff;
  da_unload(&dawr.da);
  ds_unload(&dswr.ds);
  return database;
}

void database_flush(const Database *database) {
  DatabaseMeta stored = database->dst;
  rewind(database->file);
  fwrite(&stored, sizeof(stored), 1, database->file);
  fflush(database->file);
}

void database_alter(const Database *database, const char *meta) {
  DatabaseMeta stored = database->dst;
  size_t total_size = sizeof(stored) + strlen(meta);
  assert(total_size <= stored.pos_empty && total_size <= stored.ds_first);
  // flush data (rewind + write)
  database_flush(database);
  fwrite(meta, strlen(meta), 1, database->file);
}

void database_drop(Database *database) {
  fclose(database->file);
  remove(database->name);
  free(database->name);
  database->file = NULL;
  database->name = NULL;
  database->is_opened = false;
}

Database database_open(const char *filename) {
  FILE *file = fopen(filename, "r+b");
  assert(file != NULL);
  Database database;
  // init database entry to store in RAM
  database.is_opened = true;
  database.file = file;
  database.name = strdup(filename);
  database_load(&database);
  return database;
}

void database_close(Database *database) {
  database_flush(database);
  fclose(database->file);
  free(database->name);
  database->file = NULL;
  database->name = NULL;
  database->is_opened = false;
}

void database_remove(Database *database) { database_drop(database); }
