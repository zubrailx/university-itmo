#include "../src/dbms/dto/dto_row.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/op_dbms.c"
#include "../src/dbms/plan.h"
#include "../src/schema.h"
#include "../src/table.h"
#include "../src/util/printers.h"
#include <database.h>

int main() {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *table = dto_table_construct((char *)"table1");
  dto_table_add_column(table, (char *)"column1", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(table, (char *)"column2", DTO_COLUMN_BOOL,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column3", DTO_COLUMN_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(table, (char *)"column4", DTO_COLUMN_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, table);

  const char *row1_1 = "hello";
  bool row1_2 = true;
  double row1_4 = 31.1F;
  int32_t row1_3 = 5;

  const void *row[] = {row1_1, &row1_2, &row1_3, &row1_4};
  dto_row_list list = dto_row_list_construct();

  int cnt = 200;
  for (int i = 0; i < cnt; ++i) {
    dto_row_list_append(&list, row);
  }
  row_list_insert(dbms, "table1", &list);

  // TEST source (passed)
  // struct plan_source tsource = plan_source_construct("table1", dbms);
  // const struct plan_table_info *info = tsource.get_info(&tsource);
  // while (!tsource.end(&tsource)) {
  //   tp_tuple **tuple_list = tsource.get(&tsource);
  //   print_table_tuple(tuple_list[0], info[0].dpt, info[0].col_info);
  //   tsource.next(&tsource);
  // }
  // tsource.destruct(&tsource);

  // TEST select (failed)
  struct plan_select *select_table1 =
      plan_select_construct_move(plan_source_construct("table1", dbms), "temp-table1");

  const struct plan_table_info *table_info = select_table1->get_info(select_table1);

  while (!select_table1->end(select_table1)) {
    tp_tuple **tpt = select_table1->get(select_table1);
    print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
    select_table1->next(select_table1);
  }
  select_table1->destruct(select_table1);

  print_table_rows(dbms, "table1");

  dto_row_list_destruct(&list);
  dto_table_destruct(&table);
  dbms_close(&dbms);
}
