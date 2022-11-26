#include "../src/table.h"
#include <database.h>

int main() {
  struct dbms *dbms = dbms_create("tmp/db.bin");
  dbms_close(&dbms);
}
