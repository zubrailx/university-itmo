#include "gtest/gtest.h"

extern "C" {
#include "../src/dbms/core/dbfile.h"
#include "../src/dbms/core/dbmeta.h"
#include "../src/dbms/core/dbms.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/io/p_base.h"
#include "../src/dbms/iter.h"
#include "../src/dbms/page.h"
#include "../src/dbms/pagealloc.h"
#include "../src/table.h"
#include "database.h"
#include <database.h>
}

int test(int argc, char **argv) { return 0; }

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
