#include "dbms/op_dbms.h"
#include <stdbool.h>

struct dbms *database_open(const char *fname, bool do_trunc) {
  struct dbms *dbms;
  if (do_trunc) {
    dbms = dbms_create(fname);
  } else {
    dbms = dbms_open(fname, false);
  }
  return dbms;
}

void database_close(struct dbms **dbms, bool do_remove) {
  if (do_remove) {
    dbms_remove(dbms);
  } else {
    dbms_close(dbms);
  }
}
