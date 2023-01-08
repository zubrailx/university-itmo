#include "dbfile.h"

#include <dbms/util/define.h>

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

// Getters
FILE *dbfile_file(dbfile *dbfile) { return dbfile->file; }
const char *dbfile_name(dbfile *dbfile) { return dbfile->fname; }

// Construct-destruct
dbfile *dbfile_construct(const char *fname, bool should_exist, bool do_trunc) {
  dbfile *dfile = my_malloc(dbfile);
  dfile->fname = strdup(fname);

  bool exists = !access(fname, F_OK);
  dfile->is_new = do_trunc || !exists;

  if (should_exist && !exists) {
    dfile->file = NULL;
  } else {
    if (do_trunc || !exists) {
      dfile->file = fopen(fname, "w+b");
    } else {
      dfile->file = fopen(fname, "r+b");
    }
  }

  if (!dfile->file) {
    dbfile_destruct(&dfile);
  }
  return dfile;
}

int dbfile_destruct(dbfile **ptr) {
  dbfile *dfile = *ptr;
  if (dfile) {
    dbfile_flush(dfile);
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
