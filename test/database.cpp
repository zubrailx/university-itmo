#include <gtest/gtest.h>

#include <unistd.h>

extern "C" {
#include "../src/dbms/dbms.h"
#include "../src/dbms/meta.h"
#include "../src/dbms/dbfile.h"
#include <database.h>
}

TEST(database_public, create) {
  dbms *db = dbms_create("tmp/dbms.bin");
  EXPECT_EQ(db->meta->pos_empty.bytes, sizeof(dbmeta));
  dbms_close(&db);
  EXPECT_EQ(db, nullptr);
  std::remove("tmp/dbms.bin");
}

TEST(database_public, open) {
  dbms *db = dbms_create("tmp/dbms.bin");
  dbms_close(&db);
  db = dbms_open("tmp/dbms.bin");
  printf("%zu, %zu, %zu, %zu\n", db->meta->da.first.bytes, db->meta->da.last.bytes,
         db->meta->dp.first.bytes, db->meta->dp.last.bytes);
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
