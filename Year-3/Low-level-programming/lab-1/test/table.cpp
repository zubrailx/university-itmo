#include <gtest/gtest.h>

extern "C" {
#include "sections/database.h"
#include "database.h"
}

TEST(DatabaseSection, Create) {
  Database *database = database_create("tmp/database.bin");
  dt_create()
}
