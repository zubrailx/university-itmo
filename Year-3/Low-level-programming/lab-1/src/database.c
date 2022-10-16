#include "sections/database.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

const char META_INFO[] = "version-DEV:zubrailx";

// needs file to read
static void meta_load(Database *database) {
	DatabaseMeta stored;
	rewind(database->file);
	assert(fread(&stored, sizeof(stored), 1, database->file));
	database->dst = stored;
}

Database database_create(const char *filename) {
	FILE *file = fopen(filename, "w+b");
	assert(file != NULL);

	fileoff_t offset = sizeof(DatabaseMeta) + strlen(META_INFO);
	// init database entry to store in RAM
	Database database = {.file = file,
											 .name = strdup(filename),
											 .is_opened = true,
											 .dst = (DatabaseMeta){
													 .is_corrupted = false,
													 .ds_first = offset,
													 .ds_last = offset,
													 .pos_empty = offset,
											 }};
	// create first database section
	DatabaseSection *ds = ds_create(&database, NULL, 0).ds;
  // create first data section
  DataSection *da = da_create(&database);
	ds_unload(&ds);
	return database;
}

void database_alter(const Database *database, const char *meta) {
	DatabaseMeta stored = database->dst;
	size_t total_size = sizeof(stored) + strlen(meta);
	assert(total_size <= stored.pos_empty && total_size <= stored.ds_first);
	// write meta inf
	FILE *file = database->file;
	rewind(file);
	fwrite(&stored, sizeof(stored), 1, file);
	fwrite(meta, strlen(meta), 1, file);
}

void database_drop(Database *database) {
	fclose(database->file);
	remove(database->name);
	free(database->name);
	database->file = NULL;
	database->name = NULL;
	database->is_opened = false;
}

Database database_open(const char *filename) {
	FILE *file = fopen(filename, "r+b");
	assert(file != NULL);
	Database database;
	// init database entry to store in RAM
	database.is_opened = true;
	database.file = file;
	database.name = strdup(filename);
	meta_load(&database);
	return database;
}

void database_close(Database *database) {
	database_alter(database, META_INFO);
	fclose(database->file);
	free(database->name);
	database->file = NULL;
	database->name = NULL;
	database->is_opened = false;
}

void database_remove(Database *database) { database_drop(database); }
