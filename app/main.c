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

  int cnt = 5;
  for (int i = 0; i < cnt; ++i) {
    dto_row_list_append(&list, row);
  }
  row_list_insert(dbms, "table1", &list);
  print_table_rows(dbms, "table1");

  // TEST select (passed)
  {
    printf("Selection: \n");
    struct plan_select *select_table1 = plan_select_construct_move(
        plan_source_construct("table1", dbms), "temp-table1");

    const struct plan_table_info *table_info = select_table1->get_info(select_table1);

    select_table1->start(select_table1);
    while (!select_table1->end(select_table1)) {
      tp_tuple **tpt = select_table1->get(select_table1);
      print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
      select_table1->next(select_table1);
    }
    select_table1->destruct(select_table1);

    print_table_rows(dbms, "table1");
  }

  // TEST update ()
  {
    printf("Update: \n");
    struct column_value *arr = malloc(sizeof(struct column_value) * 2);
    arr[0].column_name = "column1";
    arr[0].column_value = "goodbye dfjassdal jfklasf jasf jsafjsa jf";

    arr[1].column_name = "column3";
    arr[1].column_value = "shit";

    struct plan_update *update_table1 =
        plan_update_construct_move(plan_source_construct("table1", dbms), 2, arr);

    const struct plan_table_info *table_info = update_table1->get_info(update_table1);

    update_table1->start(update_table1);
    while (!update_table1->end(update_table1)) {
      tp_tuple **tpt = update_table1->get(update_table1);
      print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
      update_table1->next(update_table1);
    }
    update_table1->destruct(update_table1);
    free(arr);
  }

  // TEST delete ()
  {
    printf("After deletion:\n");
    struct plan_delete *delete_table1 =
        plan_delete_construct_move(plan_source_construct("table1", dbms));

    const struct plan_table_info *table_info = delete_table1->get_info(delete_table1);

    delete_table1->start(delete_table1);
    while (!delete_table1->end(delete_table1)) {
      tp_tuple **tpt = delete_table1->get(delete_table1);
      print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
      delete_table1->next(delete_table1);
    }
    delete_table1->destruct(delete_table1);
  }
  // TEST select (passed)
  {
    printf("Selection: \n");
    struct plan_select *select_table1 = plan_select_construct_move(
        plan_source_construct("table1", dbms), "temp-table1");

    const struct plan_table_info *table_info = select_table1->get_info(select_table1);

    select_table1->start(select_table1);
    while (!select_table1->end(select_table1)) {
      tp_tuple **tpt = select_table1->get(select_table1);
      print_table_tuple(tpt[0], table_info[0].dpt, table_info[0].col_info);
      select_table1->next(select_table1);
    }
    select_table1->destruct(select_table1);

    print_table_rows(dbms, "table1");
  }

  dto_row_list_destruct(&list);
  dto_table_destruct(&table);
  dbms_close(&dbms);
}
