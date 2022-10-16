#pragma once

#include "../database.h"

#include <inttypes.h>
#include <stdbool.h>

struct BaseSection {
	uint8_t type;
	uint32_t size;
};

struct SOPointer {
	fileoff_t sect_address;
	sectoff_t offset;
};

enum SectionTypes {
	TYPE_UNKNOWN,
	TYPE_DATABASE,
	TYPE_HASHTABLE,
	TYPE_BACKET,
	TYPE_PAGE,
	TYPE_PAGEDATA,
	TYPE_TABLE,
	TYPE_TMP,
	TYPE_DUMPED,
};

my_defstruct(BaseSection);
my_defstruct(SOPointer);

void *section_malloc(const sectoff_t sect_size);
void *section_load(const Database *database, const fileoff_t offset);
void section_unload(void **section);
