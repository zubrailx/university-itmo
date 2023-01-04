#include "gtest/gtest.h"
#include <fstream>

extern "C" {
#include "../src/dbms/dto/dto_row.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/op_dbms.c"
#include "../src/util/printers.h"
#include <database.h>
#include <dto_row.h>
#include <dto_table.h>
#include <plan.h>
#include <plan_filter.h>
#include <plan_funcs.h>
#include <schema.h>
#include <table.h>
#include <util/column_types.h>
}


TEST(performance, insert_one) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t1, (char *)"weight2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t1, (char *)"weight3", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nikita";
  double e2 = 100;
  int32_t o1 = 100;
  double o2 = 100;

  const void *r1[] = {e1, &e2, &o1, &o2};

  dto_row_list l1 = dto_row_list_construct();
  dto_row_list_append(&l1, r1);

  size_t units = 1'000'000;
  std::ofstream ofile("tmp/insert_one.log");
  for (size_t i = 0; i < 100000; ++i) {
    // start
    clock_t time = clock();

    row_list_insert(dbms, "table1", &l1);
    // end
    time = clock() - time;
    double time_taken = ((double)time) / CLOCKS_PER_SEC * units;
    size_t file_size = dbms->meta->pos_empty.bytes;
    ofile << i << ";" << time_taken << ";" << file_size << std::endl;
  }
  ofile.close();

  dto_row_list_destruct(&l1);

  dbms_remove(&dbms);
}

TEST(performance, select) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique =
                       false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique =
                       true});
  dto_table_add_column(t1, (char *)"weight2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique =
                       true});
  dto_table_add_column(t1, (char *)"weight3", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique =
                       true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "wasd";
  double e2 = 100;
  int32_t o1 = 100;
  double o2 = 100;

  const void *r1[] = {e1, &e2, &o1, &o2};

  std::ofstream ofile("tmp/select.txt");
  size_t units = 1'000'000;

  for (size_t i = 0; i < 100; ++i) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000; ++i) {
      dto_row_list_append(&l1, r1);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    // start
    clock_t time = clock();
    {// select
      struct plan_select *select_table1 = plan_select_construct_move(
          plan_source_construct("table1", dbms), "temp-table1");

      select_table1->start(select_table1);
      while (!select_table1->end(select_table1)) {
        select_table1->next(select_table1);
      }
      select_table1->destruct(select_table1);
    }

    // end
    time = clock() - time;
    double time_taken = ((double)time) / CLOCKS_PER_SEC * units;
    ofile << i << ";" << time_taken  << std::endl;
  }

  ofile.close();
  dbms_remove(&dbms);
}

TEST(performance, select_optional) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t1, (char *)"weight2", COLUMN_TYPE_INT32,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});
  dto_table_add_column(t1, (char *)"weight3", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "wasd";
  double e2 = 100;
  int32_t o1 = 100;
  double o2 = 100;

  const void *r1[] = {e1, &e2, &o1, &o2};

  std::ofstream ofile("tmp/select_optional.txt");
  size_t units = 1'000'000;

  for (size_t i = 0; i < 100; ++i) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000; ++i) {
      dto_row_list_append(&l1, r1);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    // start
    clock_t time = clock();
    {// select

      auto *so = plan_source_construct("table1", dbms);

      // filter
      auto fc_name = fast_column_construct("table1", "name", dbms);

      auto fc_name_nikit = fast_const_construct(COLUMN_TYPE_STRING, "nikita", dbms);

      auto fc_name_eq_nikit =
          fast_binop_construct(fc_name, fc_name_nikit, &STRING_EQUALS, dbms);

      auto fc_weight = fast_column_construct("table1", "weight", dbms);

      const double min_val = 80;
      auto *fc_weight_80 = fast_const_construct(COLUMN_TYPE_DOUBLE, &min_val, dbms);

      auto *fc_weight_le_80 =
          fast_binop_construct(fc_weight, fc_weight_80, &DOUBLE_LARGER, dbms);

      auto *fc_and =
          fast_binop_construct(fc_weight_le_80, fc_name_eq_nikit, &BOOL_AND, dbms);

      auto *pf = plan_filter_construct_move(so, fc_and);

      struct plan_select *select_table1 = plan_select_construct_move(pf, "temp-table1");

      select_table1->start(select_table1);
      while (!select_table1->end(select_table1)) {
        select_table1->next(select_table1);
      }
      select_table1->destruct(select_table1);
    }

    // end
    time = clock() - time;
    double time_taken = ((double)time) / CLOCKS_PER_SEC * units;
    ofile << i << ";" << time_taken << std::endl;
  }

  ofile.close();
  dbms_remove(&dbms);
}


TEST(performance, update_optional) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nikit";
  double e2 = 100;

  const char *e3 = "dkfakf";
  double e4 = 70;

  const void *r1[] = {e1, &e2};
  const void *r2[] = {e3, &e4};

  std::ofstream ofile("tmp/update_optional.txt");
  size_t units = 1'000'000;

  for (size_t i = 0; i < 100; ++i) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000 / 4; ++i) {
      dto_row_list_append(&l1, r1);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    // start
    clock_t time = clock();
    {// select

      auto *ps = plan_source_construct("table1", dbms);

      struct plan_filter *pf;
      {
        auto fc_name = fast_column_construct("table1", "name", dbms);

        auto fc_name_nikit = fast_const_construct(COLUMN_TYPE_STRING, "nikita", dbms);

        auto fc_name_eq_nikit =
            fast_binop_construct(fc_name, fc_name_nikit, &STRING_EQUALS, dbms);

        auto fc_weight = fast_column_construct("table1", "weight", dbms);

        const double min_val = 80;
        auto *fc_weight_80 = fast_const_construct(COLUMN_TYPE_DOUBLE, &min_val, dbms);

        auto *fc_weight_le_80 =
            fast_binop_construct(fc_weight, fc_weight_80, &DOUBLE_LARGER, dbms);

        auto *fc_and =
            fast_binop_construct(fc_weight_le_80, fc_name_eq_nikit, &BOOL_AND, dbms);

        pf = plan_filter_construct_move(ps, fc_and);
      }

      struct plan_update *pu;
      {
        size_t cols = 1;
        column_value *col_val_arr = (column_value *)malloc(sizeof(column_value) * cols);

        col_val_arr[0].column_name = "name";
        col_val_arr[0].column_value = "heavy";

        pu = plan_update_construct_move(pf, cols, col_val_arr);
        free(col_val_arr);
      }

      pu->start(pu);
      while (!pu->end(pu)) {
        pu->next(pu);
      }
      pu->destruct(pu);
    }

    // end
    time = clock() - time;
    double time_taken = ((double)time) / CLOCKS_PER_SEC * units;
    ofile << i << ";" << time_taken << std::endl;
  }

  ofile.close();
  dbms_remove(&dbms);
}

TEST(performance, delete_optional) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nikit";
  double e2 = 100;

  const char *e3 = "dkfakf";
  double e4 = 70;

  const void *r1[] = {e1, &e2};
  const void *r2[] = {e3, &e4};

  std::ofstream ofile("tmp/delete_optional.txt");
  size_t units = 1'000'000;

  for (size_t i = 0; i < 100; ++i) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000 / 4; ++i) {
      dto_row_list_append(&l1, r1);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    // start
    clock_t time = clock();
    {// select

      auto *ps = plan_source_construct("table1", dbms);

      struct plan_filter *pf;
      {
        auto fc_name = fast_column_construct("table1", "name", dbms);

        auto fc_name_nikit = fast_const_construct(COLUMN_TYPE_STRING, "nikita", dbms);

        auto fc_name_eq_nikit =
            fast_binop_construct(fc_name, fc_name_nikit, &STRING_EQUALS, dbms);

        auto fc_weight = fast_column_construct("table1", "weight", dbms);

        const double min_val = 80;
        auto *fc_weight_80 = fast_const_construct(COLUMN_TYPE_DOUBLE, &min_val, dbms);

        auto *fc_weight_le_80 =
            fast_binop_construct(fc_weight, fc_weight_80, &DOUBLE_LARGER, dbms);

        auto *fc_and =
            fast_binop_construct(fc_weight_le_80, fc_name_eq_nikit, &BOOL_AND, dbms);

        pf = plan_filter_construct_move(ps, fc_and);
      }

      struct plan_delete *pu;
      {
        pu = plan_delete_construct_move(pf);
      }

      pu->start(pu);
      while (!pu->end(pu)) {
        pu->next(pu);
      }
      pu->destruct(pu);
    }

    // end
    time = clock() - time;
    double time_taken = ((double)time) / CLOCKS_PER_SEC * units;
    ofile << i << ";" << time_taken << std::endl;
  }

  ofile.close();
  dbms_remove(&dbms);
}

TEST(performance, size_delete_insert) {
  dbms *dbms = dbms_create("tmp/db.bin");

  dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", COLUMN_TYPE_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});
  dto_table_add_column(t1, (char *)"weight", COLUMN_TYPE_DOUBLE,
                       (dto_table_column_limits){.is_null = false, .is_unique = true});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nikita";
  double e2 = 100;

  const char *e3 = "dkfakf";
  double e4 = 70;

  const void *r1[] = {e1, &e2};
  const void *r2[] = {e3, &e4};

  std::ofstream ofile("tmp/size_delete_insert.txt");

  size_t idx = 0;
  for (size_t i = 0; i < 100; ++i, ++idx) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000 / 4; ++i) {
      dto_row_list_append(&l1, r1);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    ofile << idx << ";" << dbms->meta->pos_empty.bytes << std::endl;
  }

  {// delete 1/4 of rows

    auto *ps = plan_source_construct("table1", dbms);

    struct plan_filter *pf;
    {
      auto fc_name = fast_column_construct("table1", "name", dbms);

      auto fc_name_nikit = fast_const_construct(COLUMN_TYPE_STRING, "nikita", dbms);

      auto fc_name_eq_nikit =
          fast_binop_construct(fc_name, fc_name_nikit, &STRING_EQUALS, dbms);

      auto fc_weight = fast_column_construct("table1", "weight", dbms);

      const double min_val = 80;
      auto *fc_weight_80 = fast_const_construct(COLUMN_TYPE_DOUBLE, &min_val, dbms);

      auto *fc_weight_le_80 =
          fast_binop_construct(fc_weight, fc_weight_80, &DOUBLE_LARGER, dbms);

      auto *fc_and =
          fast_binop_construct(fc_weight_le_80, fc_name_eq_nikit, &BOOL_AND, dbms);

      pf = plan_filter_construct_move(ps, fc_and);
    }

    struct plan_delete *pu;
    {
      pu = plan_delete_construct_move(pf);
    }

    pu->start(pu);
    while (!pu->end(pu)) {
      pu->next(pu);
    }
    pu->destruct(pu);
  }

  for (size_t i = 0; i < 50; ++i, ++idx) {
    // insert
    dto_row_list l1 = dto_row_list_construct();
    for (size_t i = 0; i < 10000 / 4; ++i) {
      dto_row_list_append(&l1, r1);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
      dto_row_list_append(&l1, r2);
    }
    row_list_insert(dbms, "table1", &l1);
    dto_row_list_destruct(&l1);

    ofile << idx << ";" << dbms->meta->pos_empty.bytes << std::endl;
  }

  ofile.close();
  dbms_remove(&dbms);
}
