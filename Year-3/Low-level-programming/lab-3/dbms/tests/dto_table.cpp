#include <gtest/gtest.h>

extern "C" {
#include <dbms/dto_table.h>
}

TEST(dto_table, create) {
  dto_table *table = dto_table_construct((char *)"table1");
  dto_table_add_column(table, (char *)"column1", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_destruct(&table);
}
