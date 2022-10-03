#include "database_meta.h"
#include "sections/table.h"

int main() {
  Database database = database_create("tmp/first.db.bin");
  // Database database = database_open("tmp/first.db.bin");
  // printf("%p\n", database_table_select(&database, "table1"));
  database_close(&database);
  return 0;
}
