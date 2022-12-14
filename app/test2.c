#include "../src/dbms/dto/dto_row.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/op_dbms.c"
#include "../src/util/printers.h"
#include <database.h>
#include <dto_table.h>
#include <plan.h>
#include <plan_filter.h>
#include <dto_row.h>
#include <util/column_types.h>
#include <plan_funcs.h>
#include <schema.h>
#include <table.h>

int main() {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nikita";
  double e2 = 100;
  const char *e3 = "andrey";
  double e4 = 90;
  const char *e5 = "nikita";
  double e6 = 80;
  const char *e7 = "alexander";
  double e8 = 70;

  const void *r1[] = {e1, &e2};
  const void *r2[] = {e3, &e4};
  const void *r3[] = {e5, &e6};
  const void *r4[] = {e7, &e8};

  dto_row_list l1 = dto_row_list_construct();
  dto_row_list_append(&l1, r1);
  dto_row_list_append(&l1, r2);
  dto_row_list_append(&l1, r3);
  dto_row_list_append(&l1, r4);
  row_list_insert(dbms, "table1", &l1);
  dto_row_list_destruct(&l1);

  printf("ALL FROM TABLE: \n");
  print_table_rows(dbms, "table1");

  {
    struct plan_source *so1 = plan_source_construct("table1", dbms);
    // struct plan_source *so2 = plan_source_construct("table2", dbms);
    // struct plan_source *so3 = plan_source_construct("table1", dbms);
    // struct plan_cross_join *j1 = plan_cross_join_construct_move(so1, so2);
    // struct plan_cross_join *j2 = plan_cross_join_construct_move(j1, so3);

    struct fast_column *fc_name = fast_column_construct("table1", "name", dbms);

    struct fast_const *fc_nikit =
        fast_const_construct(COLUMN_TYPE_STRING, "nikita", dbms);

    struct fast_column *fc_weight = fast_column_construct("table1", "weight", dbms);
    struct fast_binop *fb1 =
        fast_binop_construct(fc_name, fc_nikit, &STRING_EQUALS, dbms);

    const double min_val = 80;
    struct fast_const *fc_weight80 =
        fast_const_construct(COLUMN_TYPE_DOUBLE, &min_val, dbms);

    struct fast_binop *fc_max_wight =
        fast_binop_construct(fc_weight, fc_weight80, &DOUBLE_LARGER, dbms);

    struct fast_binop *fc_and =
        fast_binop_construct(fc_max_wight, fb1, &BOOL_AND, dbms);

    // struct plan_filter *f = plan_filter_construct_move(j1, fb1);
    struct column_value *cols = malloc(sizeof(struct column_value));
    cols[0].column_name = "name";
    cols[0].column_value = "perestaralsiya";

    struct plan_filter *fi = plan_filter_construct_move(so1, fc_and);
    struct plan_update *se = plan_update_construct_move(fi, 1, cols);

    size_t ti_size;
    struct plan_table_info ti = se->get_info(se, &ti_size)[0];

    se->start(se);
    while (!se->end(se)) {
      tp_tuple *tpt = se->get(se)[0];
      print_table_tuple(tpt, ti.dpt, ti.col_info, dbms);
      se->next(se);
    }
    se->destruct(se);
  }
  printf("-----------------------\nAfter:\n");
  print_table_rows(dbms, "table1");

  dbms_close(&dbms);
}
