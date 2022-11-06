#include <gtest/gtest.h>

#include <unistd.h>

extern "C" {
#include "../src/database.h"
#include <database.h>
}

void print_database(const database *database) {
  printf("%d, %s, %lu, %d, %lu, %lu\n", database->is_opened, database->fname,
         database->dst->dp.first, database->dst->is_corrupted, database->dst->dp.last,
         database->dst->pos_empty);
}

TEST(database_public, create) {
  database *database = database_create("tmp/db.bin");
  // print_database(database);
  database_close(&database);
  EXPECT_EQ(database, nullptr);
}

TEST(database_public, open) {
  char name[] = "tmp/db.bin";
  database *db = database_create(name);
  database_close(&db);

  database *db2 = database_open(name, true);
  database_close(&db2);
}

TEST(database_public, remove) {
  const char *name = "tmp/db.bin";
  database *db = database_create(name);
  EXPECT_EQ(access(name, F_OK), 0);
  database_remove(&db);
  EXPECT_EQ(db, nullptr);
}

TEST(database_public, runtime) {
  const char *name = "tmp/db.bin";
  database *db = database_create(name);
  EXPECT_STREQ(database_get_name(db), name);
  EXPECT_EQ(database_is_opened(db), true);
  EXPECT_EQ(database_is_writable(db), true);
  database_close(&db);
}
