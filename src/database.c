#include <database.h>

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "database.h"
#include "page/data.h"
#include "page/database.h"

const char META_INFO[] = "version-DEV:zubrailx";

// needs file to read
static void database_load(database *database) {
  database_meta stored;
  rewind(database->file);
  assert(fread(&stored, sizeof(stored), 1, database->file));
  database->dst = stored;
}

static void database_create_pages(database *database) {
  // Create first database_section
  database_page_wr dswr = dp_create(database, NULL, 0);
  database->dst.dp.first = dswr.fileoff;
  database->dst.dp.last = dswr.fileoff;
  // Create first data section
  data_page_wr dawr = da_create(database);
  database->dst.da.first = dswr.fileoff;
  database->dst.da.last = dswr.fileoff;

  da_unload(&dawr.da);
  dp_unload(&dswr.dp);
}

database *database_create(const char *filename) {
  FILE *file = fopen(filename, "w+b");
  assert(file != NULL);

  fileoff_t offset = sizeof(database_meta) + strlen(META_INFO);
  // init database entry to store in RAM
  database *db = my_malloc(database);
  *db = (database){.file = file,
                   .name = strdup(filename),
                   .is_opened = true,
                   .dst = (database_meta){
                       .is_corrupted = false,
                       .pos_empty = offset,
                   }};
  database_create_pages(db);
  return db;
}

void database_flush(const database *database) {
  database_meta stored = database->dst;
  rewind(database->file);
  fwrite(&stored, sizeof(stored), 1, database->file);
  fflush(database->file);
}

void database_alter(const database *database, const char *meta) {
  database_meta stored = database->dst;
  size_t total_size = sizeof(stored) + strlen(meta);
  assert(total_size <= stored.pos_empty && total_size <= stored.dp.first);
  // flush data (rewind + write)
  database_flush(database);
  fwrite(meta, strlen(meta), 1, database->file);
}

static void database_free(database **db_ptr) {
  database *database = *db_ptr;
  free(database->name);
  free(database);
  *db_ptr = NULL;
}

void database_drop(database **db_ptr) {
  database *database = *db_ptr;
  fclose(database->file);
  remove(database->name);
  database_free(db_ptr);
}

database *database_open(const char *filename) {
  FILE *file = fopen(filename, "r+b");
  assert(file != NULL);
  database *db = my_malloc(database);
  // init database entry to store in RAM
  db->is_opened = true;
  db->file = file;
  db->name = strdup(filename);
  database_load(db);
  return db;
}

void database_close(database **db_ptr) {
  database *database = *db_ptr;
  database_flush(database);
  fclose(database->file);
  database_free(db_ptr);
}

void database_remove(database **database) { database_drop(database); }
