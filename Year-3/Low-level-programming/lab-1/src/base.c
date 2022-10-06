#include "sections/base.h"

#include <assert.h>
#include <malloc.h>

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
