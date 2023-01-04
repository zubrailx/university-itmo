#include "dbfile.h"

#include <util/define.h>

#include <malloc.h>
#include <string.h>

// Getters
FILE *dbfile_file(dbfile *dbfile) { return dbfile->file; }
const char *dbfile_name(dbfile *dbfile) { return dbfile->fname; }
bool dbfile_is_opened(dbfile *dbfile) { return dbfile->is_opened; }
bool dbfile_is_writable(dbfile *dbfile) { return dbfile->is_writable; }

// Construct-destruct
dbfile *dbfile_construct(const char *fname, bool do_trunc) {
  dbfile *dfile = my_malloc(dbfile);
  dfile->fname = strdup(fname);
  if (do_trunc) {
    dfile->file = fopen(fname, "w+b");
  } else {
    dfile->file = fopen(fname, "r+b");
  }
  dfile->is_opened = dfile->file;
  dfile->is_writable = true;
  return dfile;
}

void dbfile_destruct(dbfile **ptr) {
  dbfile *dfile = *ptr;
  dbfile_close(dfile);
  free(dfile->fname);
  free(dfile);
  *ptr = NULL;
}

// Operations with file
bool dbfile_flush(dbfile *dbfile) {
  if (dbfile->is_opened && dbfile->is_writable) {
    fflush(dbfile->file);
    return true;
  }
  return false;
}

bool dbfile_close(dbfile *dbfile) {
  if (dbfile->is_opened) {
    fclose(dbfile->file);
    dbfile->file = NULL;
    dbfile->is_opened = false;
    return true;
  }
  return false;
}

void dbfile_remove(dbfile *dbfile) {
  dbfile_close(dbfile);
  remove(dbfile->fname);
}
