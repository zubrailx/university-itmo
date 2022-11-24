#include <malloc.h>

#include <database.h>

#include <util/define.h>

#include "core/dbfile.h"
#include "core/dbms.h"
#include "core/meta.h"
#include "io/meta.h"
#include "page.h"

static dbms *dbms_construct(const char *fname, bool do_trunc) {
  dbms *db = my_malloc(dbms);
  db->dbfile = dbfile_construct(fname, do_trunc);
  db->meta = meta_construct();
  return db;
}

static void dbms_destruct(dbms **dbms_ptr) {
  dbfile_destruct(&(*dbms_ptr)->dbfile);
  meta_destruct(&(*dbms_ptr)->meta);
  free(*dbms_ptr);
  *dbms_ptr = NULL;
}

// Create database and write to file
dbms *dbms_create(const char *fname) {
  dbms *dbms = dbms_construct(fname, true);
  meta_create(dbms->meta, dbms->dbfile->file);
  // create pages
  dbms_dp_create(dbms);
  dbms_da_create(dbms);
  // update meta
  meta_alter(dbms->meta, dbms->dbfile->file);
  return dbms;
}

dbms *dbms_open(const char *fname) {
  dbms *dbms = dbms_construct(fname, false);
  meta_load(dbms->meta, dbms->dbfile->file);
  return dbms;
}

static void dbms_flush(struct dbms *db) {
  meta_alter(db->meta, db->dbfile->file);
  dbfile_flush(db->dbfile);
}

void dbms_close(struct dbms **dbms_ptr) {
  dbms_flush(*dbms_ptr);
  dbms_destruct(dbms_ptr);
}
void dbms_remove(struct dbms **dbms_ptr) {
  dbfile_remove((*dbms_ptr)->dbfile);
  dbms_destruct(dbms_ptr);
}
