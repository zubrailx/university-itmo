#include "gtest/gtest.h"

extern "C" {
#include "../src/dbms/io/page/p_table.h"
#include "../src/dbms/op_dbms.h"
#include "dbms/dto_table.h"
#include "dbms/schema.h"
#include <dbms/database.h>
#include <dbms/plan.h>
}

int test(int argc, char **argv) {
  struct dbms *dbms = database_open(".tmp-db.bin", false);
  struct plan_select *select_table1 =
      plan_select_construct_move(plan_source_construct("table1", dbms), "");

  select_table1->start(select_table1);
  while (!select_table1->end(select_table1)) {
    select_table1->next(select_table1);
  }
  select_table1->destruct(select_table1);
  database_close(&dbms, false);
  return 0;
}

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
