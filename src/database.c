#include <database.h>

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "database.h"
#include "meta/meta_io.h"
#include "page/data.h"
#include "page/database.h"

// Retrieve database information
bool database_is_opened(const database *db) { return db->is_opened; }

bool database_is_writable(const database *db) {
  return database_is_opened(db) && db->is_writable;
}

const char *database_get_name(const database *db) { return db->fname; }

FILE *database_get_file(const database *db) { return db->file; }

database_meta *database_get_meta(const database *db) { return db->dst; }

// Init database structure without database_meta
static database *database_construct_ram(const char *filename, bool is_writable) {
  database *db = my_malloc(database);
  db->fname = strdup(filename);
  if ((db->is_writable = is_writable)) {
    db->file = fopen(filename, "w+b");
  } else {
    db->file = fopen(filename, "rb");
  }
  db->is_opened = !!db->file;
  return db;
}

static void database_create_pages(database *db) {
  database_meta *meta = database_get_meta(db);

  database_page_wr dswr = dp_create(db, NULL, FILEOFF_NULL);
  meta->dp.first = dswr.fileoff;
  meta->dp.last = dswr.fileoff;
  // Create first data section
  data_page_wr dawr = da_create(db);
  meta->da.first = dswr.fileoff;
  meta->da.last = dswr.fileoff;

  da_unload(&dawr.da);
  dp_unload(&dswr.dp);
}

// Operations with database
database *database_create(const char *filename) {
  database *db = database_construct_ram(filename, true);
  db->dst = meta_create(db, "version-DEV:zubrailx");
  database_create_pages(db);
  return db;
}

void database_flush(const database *db) {
  FILE *file = database_get_file(db);
  assert(file != NULL);
  fflush(database_get_file(db));
}
void database_alter(const database *db) { meta_alter(db); }

static void database_free(database **db_ptr) {
  database *db = *db_ptr;
  free(db->fname);
  meta_unload(&db->dst);
  free(db);
  *db_ptr = NULL;
}

void database_drop(database **db_ptr) {
  database *db = *db_ptr;
  fclose(db->file);
  remove(db->fname);
  database_free(db_ptr);
}

database *database_open(const char *filename, bool is_writable) {
  database *db = database_construct_ram(filename, is_writable);
  db->dst = meta_load(db);
  return db;
}

void database_close(database **db_ptr) {
  database *database = *db_ptr;
  assert(database->file != NULL);
  database_flush(database);
  fclose(database->file);
  database_free(db_ptr);
}

void database_remove(database **database) { database_drop(database); }
