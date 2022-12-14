#include "../src/dbms/dto/dto_row.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/op_dbms.c"
#include "../src/dbms/plan.h"
#include "../src/dbms/plan_filter.h"
#include "../src/schema.h"
#include "../src/table.h"
#include "../src/util/printers.h"
#include <database.h>

int main() {
  dbms *dbms = dbms_create("tmp/table-create_and_drop.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"column1", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"column2", DTO_COLUMN_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t1, (char *)"column3", DTO_COLUMN_BOOL,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  dto_table *t2 = dto_table_construct((char *)"table2");
  dto_table_add_column(t2, (char *)"column1", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t2, (char *)"column2", DTO_COLUMN_BOOL,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t2, (char *)"column3", DTO_COLUMN_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  table_create(dbms, t2);
  dto_table_destruct(&t2);

  const char *e1 = "from table1";
  double e2 = 31.1F;
  bool e21 = true;
  const char *e3 = "row 2";
  double e4 = 21.1F;
  bool e41 = false;
  const char *e5 = "from table2";
  bool e6 = false;
  double e61 = 11.1F;
  const char *e7 = "row 4";
  bool e8 = true;
  double e81 = 01.1F;

  const void *r1[] = {e1, &e2, &e21};
  const void *r2[] = {e3, &e4, &e41};
  const void *r3[] = {e5, &e6, &e61};
  const void *r4[] = {e7, &e8, &e81};

  dto_row_list l1 = dto_row_list_construct();
  dto_row_list l2 = dto_row_list_construct();
  dto_row_list_append(&l1, r1);
  dto_row_list_append(&l1, r2);
  dto_row_list_append(&l2, r3);
  dto_row_list_append(&l2, r4);

  row_list_insert(dbms, "table1", &l1);
  dto_row_list_destruct(&l1);

  row_list_insert(dbms, "table2", &l2);
  dto_row_list_destruct(&l2);

  print_table_rows(dbms, "table1");
  print_table_rows(dbms, "table2");

  // TEST (join)
  {
    struct plan_source *so1 = plan_source_construct("table1", dbms);
    struct plan_source *so2 = plan_source_construct("table2", dbms);
    // struct plan_source *so3 = plan_source_construct("table1", dbms);
    struct plan_cross_join *j1 = plan_cross_join_construct_move(so1, so2);
    // struct plan_cross_join *j2 = plan_cross_join_construct_move(j1, so3);

    bool fast_val = true;
    struct fast_const *f1 = fast_const_construct(DTO_COLUMN_BOOL, &fast_val);
    // struct fast_column *f2 = fast_column_construct("table1", "column3", dbms);

    struct plan_filter *f = plan_filter_construct_move(j1, f1);
    struct plan_select *se = plan_select_construct_move(f, "virt-joined");

    size_t t1_size;
    const struct plan_table_info ti = se->get_info(se, &t1_size)[0];

    se->start(se);
    while (!se->end(se)) {
      tp_tuple *tpt = se->get(se)[0];
      print_table_tuple(tpt, ti.dpt, ti.col_info, dbms);
      se->next(se);
    }
    se->destruct(se);
  }

  dbms_close(&dbms);
}
