#include "gtest/gtest.h"

extern "C" {
#include "sections/database.h"
}

int main(int argc, char **argv) {
  Database database = database_create("tmp/test.bin");
	// database.dst.is_corrupted = true;
	DSTColumnRAM column1 = ds_typle_column_ram_create(
			COLUMN_TYPE_BOOL, (DSTColumnLimits){}, (char *)"column1", 7);
	DSTColumnRAM column2 = ds_typle_column_ram_create(
			COLUMN_TYPE_INT, (DSTColumnLimits){}, (char *)"column2", 7);
	column1.next = &column2;
	DSTypleRAM ram = ds_typle_ram_create((char *)"table", 5, &column1, 2);
	ds_table_create(&database, &ram);
  //
	database_flush(&database);
	database_close(&database);
	// testing::InitGoogleTest(&argc, argv);
	// return RUN_ALL_TESTS();
}
