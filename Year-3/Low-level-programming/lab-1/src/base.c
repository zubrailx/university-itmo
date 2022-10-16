#include "sections/base.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

void *section_malloc(const sectoff_t sect_size) { return malloc(sect_size); }

void *section_load(const Database *database, const fileoff_t offset) {
	FILE *file = database->file;
	BaseSection base;
	fseek(file, (long)offset, SEEK_SET);
	assert(fread(&base, sizeof(base), 1, file));
	void *section = section_malloc(base.size);
	fseek(file, (long)offset, SEEK_SET);
	assert(fread(section, base.size, 1, file));
	return section;
}

void section_unload(void **base_ptr) {
	free(*base_ptr);
	*base_ptr = NULL;
}

/*
 * Returns section pos
 */
fileoff_t section_create(Database *database, const BaseSection *sect) {
	FILE *file = database->file;
	assert(file != NULL);

	fileoff_t next_pos = database->dst.pos_empty;
	database->dst.pos_empty += sect->size;

	fseek(file, (long)next_pos, SEEK_SET);
	fwrite(sect, sect->size, 1, file);
	return next_pos;
}

bool section_alter(Database *database, const fileoff_t fileoff,
									 const BaseSection *sect) {
	FILE *file = database->file;
	assert(file != NULL);

	fseek(file, (long)fileoff, SEEK_SET);
	return fwrite(sect, sect->size, 1, file);
}

bool section_alter_sectoff(Database *database, const fileoff_t fileoff,
													 const sectoff_t offset, const void *data,
													 const size_t size) {
	FILE *file = database->file;
	assert(file != NULL);

	fseek(file, (long)fileoff + (long)offset, SEEK_SET);
	return fwrite(data, size, 1, file);
}

bool section_alter_sync_sectoff(Database *database, const fileoff_t fileoff,
																const sectoff_t offset, BaseSection *sect,
																const void *data, const size_t size) {
	memcpy((void *)sect + offset, data, size);
	return section_alter_sectoff(database, fileoff, offset, data, size);
}

// Sync means that loaded structure and files are both overwritten, RAM is not deleted
bool section_sync_drop(Database *database, const fileoff_t fileoff, BaseSection *sect) {
	FILE *file = database->file;
	assert(file != NULL);

	fseek(file, (long)fileoff, SEEK_SET);
	sect->type = TYPE_DUMPED;
	return fwrite(&sect->type, sizeof(sect->type), 1, file);
}
