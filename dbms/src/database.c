#include "dbms/op_dbms.h"
#include <stdbool.h>

struct dbms *database_open(const char *fname) {
  struct dbms *dbms = dbms_open(fname);
  if (dbms) { return dbms; }
  // try to create
  dbms = dbms_create(fname);
  return dbms;
}

void database_close(struct dbms **dbms, bool do_remove) {
  if (do_remove) {
    dbms_remove(dbms);
  } else {
    dbms_close(dbms);
  }
}
