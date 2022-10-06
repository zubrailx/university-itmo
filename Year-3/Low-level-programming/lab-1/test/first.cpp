#include <gtest/gtest.h>

extern "C" {
#include "database_meta.h"
#include "sections/database.h"
#include "sections/table.h"
}

TEST(Class, Imported) {

	Database database = database_open("tmp/first.db.bin");
	// printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
	// 			 database.dst.ds_first, database.dst.is_corrupted, database.dst.ds_last,
	// 			 database.dst.pos_empty);
	// database_close(&database);

	database = database_open("tmp/first.db.bin");
	printf("%d, %s, %lu, %d, %lu, %lu\n", database.is_opened, database.name,
				 database.dst.ds_first, database.dst.is_corrupted, database.dst.ds_last,
				 database.dst.pos_empty);
	printf("%p\n", dt_select(&database, "table1").ityple);
	database_close(&database);
	printf("Hello, World");
	EXPECT_EQ(database.is_opened, false);
}
