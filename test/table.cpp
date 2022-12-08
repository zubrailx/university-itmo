#include <gtest/gtest.h>

#include "util/printers.hpp"

extern "C" {
#include "../src/dbms/dto/dto_row.h"
#include "../src/schema.h"
#include "../src/table.h"
}

TEST(table, create_and_drop) {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *table = dto_table_construct((char *)"table1");
  dto_table_add_column(table, (char *)"column1", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", DTO_COLUMN_INT,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  EXPECT_EQ(table_create(dbms, table), true);
  EXPECT_EQ(table_create(dbms, table), false);
  EXPECT_EQ(table_drop(dbms, table), true);
  EXPECT_EQ(table_create(dbms, table), true);

  const char *row1_1 = "row11";
  int row1_2 = 4;

  const void *row[] = {row1_1, &row1_2};
  dto_row_list list = dto_row_list_construct();
  dto_row_list_append(&list, row);
  dto_row_list_destruct(&list);

  dto_table_destruct(&table);
  dbms_close(&dbms);
  // std::remove("tmp/db.bin");
}
