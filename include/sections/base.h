#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "dbmeta.h"

// NOTE: First - section header offset == 0, this means that freeing section header is
// NOTE: the same as freeing section

my_defstruct(BaseSection);
struct BaseSection {
	uint8_t type;
	uint32_t size;
};

enum SectionTypes {
	TYPE_UNKNOWN,
	TYPE_DATABASE,
	TYPE_HASHTABLE,
	TYPE_BACKET,
	TYPE_PAGE,
	TYPE_DATA,
	TYPE_TABLE,
	TYPE_TMP,
	TYPE_DUMPED,
};

// RAM
BaseSection *section_malloc(const sectoff_t sect_size);
BaseSection *section_load(const Database *database, const fileoff_t offset);
BaseSection *section_load_type(const Database *database, const fileoff_t fileoff,
															 int8_t type);
void section_unload(BaseSection **section);

BaseSection *section_header_load(const Database *database, const fileoff_t fileoff,
																 size_t size);

// FILE or FILE + RAM(sync)
fileoff_t section_create(Database *database, const BaseSection *section);

bool section_alter(Database *database, const fileoff_t fileoff,
									 const BaseSection *sect);
bool section_alter_sectoff(Database *database, const fileoff_t fileoff,
													 const sectoff_t offset, const void *data, const size_t size);
bool section_alter_sync_sectoff(Database *database, const fileoff_t fileoff,
																const sectoff_t offset, BaseSection *base,
																const void *data, const size_t size);

bool section_drop(Database *database, const fileoff_t fileoff);
bool section_sync_drop(Database *database, const fileoff_t fileoff,
											 BaseSection *section);
