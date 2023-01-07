#include "gtest/gtest.h"

extern "C" {
#include "../src/dbms/core/dbfile.h"
#include "../src/dbms/core/dbmeta.h"
#include "../src/dbms/core/dbms.h"
#include "../src/dbms/op_dbms.h"
#include "../src/dbms/io/p_base.h"
#include "../src/dbms/iter.h"
#include "../src/dbms/page.h"
#include "../src/dbms/pagealloc.h"
#include <dbms/database.h>
}

TEST(pagealloc, test) {
  dbms *db = dbms_create("tmp/dbms.bin");
  database_page *page;
  int n = 100;
  page_entry entry[n];
  for (int i = 0; i < n; ++i) {
    entry[i].start = dbms_dp_create(db, SIZE_DEFAULT, &page);
    entry[i].size = page->header.base.size;
    dp_destruct(&page);
  }

  for (int i = 0; i < n; ++i) {
    dbms_page_free(db, &entry[i]);
  }
  for (int i = 0; i < n; ++i) {
    dbms_dp_create(db, SIZE_DEFAULT, &page);
    dp_destruct(&page);
  }
  dbms_close(&db);
  EXPECT_EQ(db, nullptr);
  std::remove("tmp/dbms.bin");
}
