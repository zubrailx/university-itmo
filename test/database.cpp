#include <gtest/gtest.h>

extern "C" {
// #include "../src/database.h"
#include <database.h>
}

TEST(DatabasePublic, Create) {
  database *database = database_create("tmp/db.bin");
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);
  database_close(&database);
  EXPECT_EQ(database, nullptr);
}

TEST(DatabasePublic, Open) {
  char name[] = "tmp/db.bin";
  database *db = database_create(name);
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);
  database_close(&db);

  database *db2 = database_open(name);
  // printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
  // 			 database.dst.ds_first, database.dst.is_corrupted,
  // database.dst.ds_last, 			 database.dst.pos_empty);

  database_close(&db2);
}

TEST(DatabasePublic, Remove) {
  database *database = database_create("tmp/db.bin");
  database_remove(&database);
  EXPECT_EQ(database, nullptr);
}
