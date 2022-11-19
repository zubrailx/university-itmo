#include <database.h>

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "database.h"
#include "meta/meta_io.h"

// Get/Set
bool database_is_opened(const database *db) { return db->is_opened; }

bool database_is_writable(const database *db) {
  return database_is_opened(db) && db->is_writable;
}

const char *database_get_name(const database *db) { return db->fname; }

FILE *database_get_file(const database *db) { return db->file; }

// RAM
static database *database_construct(const char *filename, bool is_writable) {
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

static void database_destruct(database **db_ptr) {
  database *db = *db_ptr;
  free(db->fname);
  free(db);
  *db_ptr = NULL;
}

// Operations with database
database *database_create(const char *filename) {
  database *db = database_construct(filename, true);
  database_meta *meta = meta_construct();
  meta_prepare_create(meta, 0, 0);
  meta_create(meta, db);
  return db;
}
