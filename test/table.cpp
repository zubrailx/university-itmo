#include <gtest/gtest.h>

extern "C" {
#include "../src/dbms/dto/table.h"
#include "../src/table.h"
#include "database.h"
}

TEST(table, create) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *table = dto_table_construct((char *)"table1");
  dto_table_add_column(table, (char *)"column1", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", DTO_COLUMN_INT,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dbms_create_table(dbms, table);
  dto_table_destruct(&table);
  dbms_close(&dbms);
}
