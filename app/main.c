#include "../src/dbms/dbms.c"
#include "../src/dbms/dto/dto_table.h"
#include "../src/table.h"
#include <database.h>

int main() {
  struct dbms *dbms = dbms_create("tmp/app.bin");

  dbms_close(&dbms);
}
