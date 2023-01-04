#include "gtest/gtest.h"

extern "C" {
#include <database.h>
}

int test(int argc, char **argv) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_close(&db);
  dbms *opened = dbms_open("tmp/dbms.bin");
  db = dbms_create("tmp/dbms2.bin");
  // EXPECT_EQ(opened->meta->pos_empty.bytes, db->meta->pos_empty.bytes);
  // EXPECT_EQ(opened->meta->da.first.bytes, db->meta->da.first.bytes);
  // EXPECT_EQ(opened->meta->da.last.bytes, db->meta->da.last.bytes);
  // EXPECT_EQ(opened->meta->dp.first.bytes, db->meta->dp.first.bytes);
  // EXPECT_EQ(opened->meta->dp.last.bytes, db->meta->dp.last.bytes);
  dbms_close(&db);
  dbms_close(&opened);
  EXPECT_EQ(db, nullptr);
  std::remove("tmp/dbms.bin");
  std::remove("tmp/dbms2.bin");
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
