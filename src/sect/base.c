#include "base.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

static BaseSection DUMPED = {.type = TYPE_DUMPED};

BaseSection *section_malloc(const sectoff_t sect_size) {
	return (BaseSection *)malloc(sect_size);
}

BaseSection *section_load(const Database *database, const fileoff_t offset) {
	FILE *file = database->file;
	BaseSection base;
	fseek(file, (long)offset, SEEK_SET);
	assert(fread(&base, sizeof(base), 1, file));
	void *section = section_malloc(base.size);
	fseek(file, (long)offset, SEEK_SET);
	assert(fread(section, base.size, 1, file));
	return section;
}

BaseSection *section_load_type(const Database *database, const fileoff_t fileoff,
															 int8_t type) {
	BaseSection *sect = (BaseSection *)section_load(database, fileoff);
	if (sect->type != TYPE_DATABASE) {
		section_unload((BaseSection **)&sect);
	}
	return sect;
}

BaseSection *section_header_load(const Database *database, const fileoff_t fileoff,
																 size_t size) {
	FILE *file = database->file;
	fseek(file, (long)fileoff, SEEK_SET);
	BaseSection *base = malloc(size);
	assert(fread(&base, size, 1, file));
	return base;
}

void section_unload(BaseSection **base_ptr) {
	free(*base_ptr);
	*base_ptr = NULL;
}

// Returns section pos
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

bool section_drop(Database *database, const fileoff_t fileoff) {
	FILE *file = database->file;
	assert(file != NULL);

	fseek(file, (long)fileoff, SEEK_SET);
	return fwrite(&DUMPED.type, sizeof(DUMPED.type), 1, file);
}
// Sync means that loaded structure and files are both overwritten, RAM is not deleted
// but header section type is set to dumped
bool section_sync_drop(Database *database, const fileoff_t fileoff, BaseSection *sect) {
	sect->type = TYPE_DUMPED;
	return section_drop(database, fileoff);
}
