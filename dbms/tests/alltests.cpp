#include "gtest/gtest.h"

extern "C" {
#include <dbms/database.h>
#include "../src/dbms/op_dbms.h"
#include "../src/dbms/io/page/p_table.h"
}

int test(int argc, char **argv) {
  printf("%zu\n", sizeof(tpt_column_COLUMN_TYPE_DOUBLE));
  return 0;
#define _ALIGNED_PADDING(offset, align) (-offset & (align - 1))
}

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
