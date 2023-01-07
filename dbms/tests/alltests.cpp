#include "gtest/gtest.h"

extern "C" {
#include <dbms/database.h>
#include "dbms/dto_table.h"
#include "dbms/schema.h"
#include "../src/dbms/op_dbms.h"
#include "../src/dbms/io/page/p_table.h"
}

int test(int argc, char **argv) {
  struct dbms *dbms = database_open(".tmp-db2.bin", false);
  struct dto_table *dto_table = dto_table_construct("t1");
  dto_table_add_column(dto_table, "col1", COLUMN_TYPE_INT32, {});
  table_create(dbms, dto_table);
  database_close(&dbms, false);
  return 0;
}

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
