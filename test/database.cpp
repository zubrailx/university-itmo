#include <gtest/gtest.h>

#include <unistd.h>

extern "C" {
#include <database.h>
#include "../src/dbms/dbms.h"
#include "../src/dbms/meta.h"
}

TEST(database_public, create) {
  dbms *db = dbms_create("tmp/dbms.bin");
  EXPECT_EQ(db->meta->pos_empty, sizeof(dbmeta));
  dbms_close(&db);
  EXPECT_EQ(db, nullptr);
  std::remove("tmp/dbms.bin");
}

TEST(database_public, open) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_close(&db);
  db = dbms_open("tmp/dbms.bin");
  printf("%zu, %zu, %zu, %zu\n", db->meta->da.first, db->meta->da.last, db->meta->dp.first, db->meta->dp.last);
  dbms_close(&db);
  std::remove("tmp/dbms.bin");
}

TEST(database_public, remove) {
  dbms *db = dbms_create("tmp/dbms.bin");
  EXPECT_EQ(db->file->is_opened, true);
  dbms_remove(&db);
  EXPECT_EQ(db, nullptr);
  EXPECT_NE(access("tmp/dbms.bin", F_OK), 0);
}
