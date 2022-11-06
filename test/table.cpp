#include <gtest/gtest.h>

extern "C" {
#include "../src/database.h"
// #include "../src/page/database.h"
}

TEST(DatabaseSection, Create) {
  database *database = database_create("tmp/database.bin");
  // char *name = strdup("column1");
  // DSTColumnRAM column = ds_typle_column_ram_create(
  //     COLUMN_TYPE_BOOL, (DSTColumnLimits){.is_null = false, .is_unique = false},
  //     name, strlen(name));
  // char *name2 = strdup("column2");
  // DSTColumnRAM column2 = ds_typle_column_ram_create(
  //     COLUMN_TYPE_INT, (DSTColumnLimits){.is_null = true, .is_unique = false}, name2,
  //     strlen(name2));
  // column.next = &column2;
  // char *table = strdup("table1");
  // DSTypleRAM typle = ds_typle_ram_create(table, strlen(table), &column, 2);
  database_close(&database);
}
