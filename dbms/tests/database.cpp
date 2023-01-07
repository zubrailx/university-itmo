#include <gtest/gtest.h>

#include <unistd.h>

extern "C" {
#include "../src/dbms/core/dbfile.h"
#include "../src/dbms/core/dbms.h"
#include "../src/dbms/op_dbms.h"
#include <dbms/database.h>
}

TEST(database_public, create) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_close(&db);
  EXPECT_EQ(db, nullptr);
  std::remove("tmp/dbms.bin");
}

TEST(database_public, open) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_close(&db);
  dbms *opened = dbms_open("tmp/dbms.bin", false);
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
}

TEST(database_public, remove) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_remove(&db);
  EXPECT_EQ(db, nullptr);
  EXPECT_NE(access("tmp/dbms.bin", F_OK), 0);
}
