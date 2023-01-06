#include "dbfile.h"

#include <dbms/util/define.h>

#include <malloc.h>
#include <string.h>

// Getters
FILE *dbfile_file(dbfile *dbfile) { return dbfile->file; }
const char *dbfile_name(dbfile *dbfile) { return dbfile->fname; }

// Construct-destruct
dbfile *dbfile_construct(const char *fname, bool do_trunc) {
  dbfile *dfile = my_malloc(dbfile);
  dfile->fname = strdup(fname);
  if (do_trunc) {
    dfile->file = fopen(fname, "w+b");
  } else {
    dfile->file = fopen(fname, "r+b");
  }
  // if file is present
  if (!dfile->file) {
    dbfile_destruct(&dfile);
  }
  return dfile;
}

int dbfile_destruct(dbfile **ptr) {
  dbfile *dfile = *ptr;
  if (dfile) {
    int res = dbfile_close(dfile);
    free(dfile->fname);
    free(dfile);
    *ptr = NULL;
    return res;
  } else {
    return -1;
  }
}

// Operations with file
int dbfile_flush(dbfile *dbfile) { return fflush(dbfile->file); }

int dbfile_close(dbfile *dbfile) {
  if (dbfile->file) {
    int res = fclose(dbfile->file);
    dbfile->file = NULL;
    return res;
  } else {
    return -1;
  }
}

int dbfile_remove(dbfile *dbfile) {
  int res = dbfile_close(dbfile);
  if (!res) {
    return remove(dbfile->fname);
  } else {
    return res;
  }
}
