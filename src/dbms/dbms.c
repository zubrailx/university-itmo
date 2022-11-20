#include "dbms.h"

#include <malloc.h>

#include "meta.h"
#include "meta_io.h"

static dbms *dbms_construct(const char *fname, bool do_trunc) {
  dbms *db = my_malloc(dbms);
  db->file = dbfile_construct(fname, do_trunc);
  db->meta = meta_construct();
  return db;
}

static void dbms_destruct(dbms **dbms_ptr) {
  dbfile_destruct(&(*dbms_ptr)->file);
  meta_destruct(&(*dbms_ptr)->meta);
  free(*dbms_ptr);
  *dbms_ptr = NULL;
} 

// Create database and write to file
dbms *dbms_create(const char *fname) {
  dbms *dbms = dbms_construct(fname, true);
  meta_create(dbms->meta, dbms->file);
  // TODO: create pages
  fileoff_t dp_addr = get_fileoff_t(1234);
  fileoff_t da_addr = get_fileoff_t(2345);
  meta_init_pages(dbms->meta, dp_addr, da_addr);
  meta_alter(dbms->meta, dbms->file);
  return dbms;
}

dbms *dbms_open(const char *fname) {
  dbms *dbms = dbms_construct(fname, false);
  meta_load(dbms->meta, dbms->file);
  return dbms;
}

static void dbms_flush(struct dbms *db) {
  meta_alter(db->meta, db->file);
  dbfile_flush(db->file);
}

void dbms_close(struct dbms **dbms_ptr) {
  dbms_flush(*dbms_ptr); 
  dbms_destruct(dbms_ptr);
}
void dbms_remove(struct dbms **dbms_ptr) {
  dbfile_remove((*dbms_ptr)->file);
  dbms_destruct(dbms_ptr);
}
