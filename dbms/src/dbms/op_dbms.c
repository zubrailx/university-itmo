#include <malloc.h>

#include <dbms/database.h>
#include <dbms/util/define.h>

#include "core/dbfile.h"
#include "core/dbmeta.h"
#include "core/dbms.h"
#include "data_dist.h"
#include "io/meta.h"
#include "page.h"
#include "pagealloc.h"

// meta
static size_t DATA_PAGE_SLOT_SIZES[] = {64, 128, 256, 512, 1024};
static size_t DATA_PAGE_SLOT_COUNT[] = {32, 16, 8, 4, 2, 1};

static meta *dbms_meta_construct() {
  size_t da_len = sizeof(DATA_PAGE_SLOT_SIZES) / sizeof(size_t);
  meta *meta = meta_construct_init(da_len, DATA_PAGE_SLOT_SIZES, DATA_PAGE_SLOT_COUNT);
  return meta;
}

static void dbms_meta_destruct(meta **meta_ptr) { meta_destruct(meta_ptr); }

static int dbms_destruct(dbms **dbms_ptr) {
  if (*dbms_ptr) {
    int res = dbfile_destruct(&(*dbms_ptr)->dbfile);
    dbms_meta_destruct(&(*dbms_ptr)->meta);
    free(*dbms_ptr);
    *dbms_ptr = NULL;
    return res;
  } else {
    return -1;
  }
}

// dbms
static dbms *dbms_construct(const char *fname, bool do_trunc) {
  dbms *db = my_malloc(dbms);
  *db = (dbms) {};

  db->dbfile = dbfile_construct(fname, do_trunc);
  if (db->dbfile) {
    db->meta = dbms_meta_construct();
  }
  if (!db->meta) {
    dbms_destruct(&db);
  }
  return db;
}

// Create database and write to file
dbms *dbms_create(const char *fname) {
  dbms *dbms = dbms_construct(fname, true);
  if (dbms) {
    meta_create(dbms->meta, dbms->dbfile->file);
    // create page allocator
    dbms_pa_create_close(dbms, FILEOFF_NULL);
    // create containers data distributer
    dbms_dd_create_close(dbms);
    // create pages
    dbms_dp_create_close(dbms, SIZE_DEFAULT);
    // update meta
    meta_alter(dbms->meta, dbms->dbfile->file);
  }
  return dbms;
}

dbms *dbms_open(const char *fname) {
  dbms *dbms = dbms_construct(fname, false);
  if (dbms) {
    meta_load(dbms->meta, dbms->dbfile->file);
  }
  return dbms;
}

static void dbms_flush(struct dbms *db) {
  meta_alter(db->meta, db->dbfile->file);
  dbfile_flush(db->dbfile);
}

void dbms_close(struct dbms **dbms_ptr) {
  if (*dbms_ptr) {
    dbms_flush(*dbms_ptr);
    dbms_destruct(dbms_ptr);
  }
}
void dbms_remove(struct dbms **dbms_ptr) {
  if (*dbms_ptr) {
    dbfile_remove((*dbms_ptr)->dbfile);
    dbms_destruct(dbms_ptr);
  }
}
