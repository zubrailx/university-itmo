#include <gtest/gtest.h>

extern "C" {
#include "../src/database.h"
}

TEST(Database, Create) {
  Database database = database_create("tmp/db.bin");
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);
  database_close(&database);
  EXPECT_EQ(database.is_opened, false);
  EXPECT_EQ(database.file, nullptr);
  EXPECT_EQ(database.name, nullptr);
}

TEST(Database, Open) {
  char name[] = "tmp/db.bin";
  Database database = database_create(name);
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);
  database_close(&database);

  Database database2 = database_open(name);
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);

  database_close(&database2);
}

TEST(Database, Remove) {
  Database database = database_create("tmp/db.bin");
  database_remove(&database);
  EXPECT_EQ(database.is_opened, false);
  EXPECT_EQ(database.name, nullptr);
  EXPECT_EQ(database.file, nullptr);
}
