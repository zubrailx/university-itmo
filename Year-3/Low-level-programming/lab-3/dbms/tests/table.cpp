#include <gtest/gtest.h>

extern "C" {
#include "../src/dbms/dto/dto_row.h"
#include "../src/util/printers.h"
#include "../src/dbms/op_dbms.h"
#include <dbms/database.h>
#include <dbms/dto_row.h>
#include <dbms/dto_table.h>
#include <dbms/schema.h>
#include <dbms/table.h>
}

TEST(table, insert_list) {
  dbms *dbms = dbms_create("tmp/table-insert_list.bin");

  dto_table *table = dto_table_construct((char *)"table1");
  // dto_table_add_column(table, (char *)"column1", DTO_COLUMN_STRING,
  //                      (dto_table_column_limits){.is_null = true, .is_unique =
  //                      false});
  dto_table_add_column(table, (char *)"column1", COLUMN_TYPE_BOOL,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", COLUMN_TYPE_BOOL,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column3", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column4", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, table);

  // const char *row1_1 = "hello, this is it ksdjfk,asj jskfj kasjf jasfj safjsajf a";
  bool row1_1 = false;
  bool row1_2 = true;
  double row1_4 = 31.1F;
  int32_t row1_3 = 5;

  const void *row[] = {&row1_1, &row1_2, &row1_3, &row1_4};
  dto_row_list *list = dto_row_list_construct();
  int n = 200;
  for (int i = 0; i < n; ++i) {
    dto_row_list_append(list, row);
  }
  printf("Operations: %u\n", n);
  row_list_insert(dbms, "table1", list);

  // print_table_rows(dbms, "table1");
  dto_row_list_destruct(&list);

  dto_table_destruct(&table);
  dbms_close(&dbms);
  // std::remove("tmp/db.bin");
}
