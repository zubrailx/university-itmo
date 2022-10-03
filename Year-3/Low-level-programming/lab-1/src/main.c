#include "database_meta.h"
#include "sections/table.h"

int main() {
  // Database database = database_create("tmp/first.db.bin");
  // printf("%d, %s, %lu, %d, %lu, %lu, %d, %lu\n", database.is_opened, database.name, database.ds.ds_first, database.ds.ds_counter, database.ds.ds_first, database.ds.ds_last, database.ds.is_corrupted, database.ds.pos_empty);
  Database database = database_open("tmp/first.db.bin");
  // printf("%p\n", table_select(&database, "table1"));
  database_close(&database);
  return 0;
}
