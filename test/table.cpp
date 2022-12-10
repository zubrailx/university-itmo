#include <gtest/gtest.h>

extern "C" {
#include "../src/dbms/dto/dto_row.h"
#include "../src/schema.h"
#include "../src/table.h"
#include "../src/util/printers.h"
#include <database.h>
}

TEST(table, insert_list) {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *table = dto_table_construct((char *)"table1");
  // dto_table_add_column(table, (char *)"column1", DTO_COLUMN_STRING,
  //                      (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column1", DTO_COLUMN_BOOL,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", DTO_COLUMN_BOOL,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column3", DTO_COLUMN_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column4", DTO_COLUMN_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, table);

  // const char *row1_1 = "hello, this is it ksdjfk,asj jskfj kasjf jasfj safjsajf a";
  bool row1_1 = false;
  bool row1_2 = true;
  double row1_4 = 31.1F;
  int32_t row1_3 = 5;

  const void *row[] = {&row1_1, &row1_2, &row1_3, &row1_4};
  dto_row_list list = dto_row_list_construct();
  for (int i = 0; i < 2000000; ++i) {
    dto_row_list_append(&list, row);
  }
  row_list_insert(dbms, "table1", &list);

  // print_table_rows(dbms, "table1");
  dto_row_list_destruct(&list);

  dto_table_destruct(&table);
  dbms_close(&dbms);
  // std::remove("tmp/db.bin");
}
