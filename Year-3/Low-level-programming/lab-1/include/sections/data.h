#pragma once

#include "sections/base.h"
#include "sections/base_util.h"

extern sectoff_t DATA_SECTION_SIZE;

// Section for storing Data (like string)
struct DaSHeader {
	struct BaseSection base;
	bodyoff_t stored_last;
};

struct DataSection {
	struct DaSHeader header;
	char *body;
};

struct DataSectionWr {
	fileoff_t fileoff;
	struct DataSection *da;
};

// typedefs
my_defstruct(DaSHeader);
my_defstruct(DataSection);
my_defstruct(DataSectionWr);

DataSectionWr da_create(Database *database);
void da_alter(Database *database, fileoff_t fileoff, DataSection *da);
void da_alter_bodyoff(Database *database, fileoff_t fileoff, bodyoff_t bodyoff,
											const void *data, size_t size);
SOPointer da_alter_append(Database *database, const void *data, size_t size);
void da_drop(Database *database, fileoff_t fileoff);

DataSection *da_load(Database *database, fileoff_t fileoff);
void da_unload(DataSection **ds);
