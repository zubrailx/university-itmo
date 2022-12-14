#include <gtest/gtest.h>

extern "C" {
#include "../src/dbms/dto/dto_table.h"
#include "../src/util/printers.h"
#include "database.h"
#include <database.h>
#include <schema.h>
}

TEST(schema, create_and_drop) {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *table =
      dto_table_construct((char *)"table1dfjafjasjfjsdalfjsaj sajfsjafjsafsaj "
                                  "sssssssend");
  dto_table_add_column(table, (char *)"column1", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  EXPECT_EQ(table_create(dbms, table), true);
  EXPECT_EQ(table_create(dbms, table), false);
  EXPECT_EQ(table_drop(dbms, table), true);
  EXPECT_EQ(table_create(dbms, table), true);

  dto_table_destruct(&table);
  dbms_close(&dbms);
  // std::remove("tmp/db.bin");
}

TEST(schema, list_inserted) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *table = dto_table_construct((char *)"table 1");
  dto_table_add_column(table, (char *)"column1", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  EXPECT_EQ(table_create(dbms, table), true);
  free(table->name);
  table->name = strdup("table 2");
  EXPECT_EQ(table_create(dbms, table), true);
  free(table->name);
  table->name = strdup("table 3");
  EXPECT_EQ(table_create(dbms, table), true);
  free(table->name);
  table->name = strdup("table 4");
  EXPECT_EQ(table_create(dbms, table), true);

  // print_database_tables(dbms);

  dto_table_destruct(&table);
  dbms_close(&dbms);

  std::remove("tmp/db.bin");
}
