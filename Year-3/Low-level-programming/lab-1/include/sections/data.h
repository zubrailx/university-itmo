#pragma once

#include "sections/base.h"

// Section for storing Data (like string)
struct DataSHeader {
	struct BaseSection base_section;
	bodyoff_t stored_last;
};

struct DataSection {
	struct DataSHeader header;
	char *body;
};

// typedefs
my_defstruct(DATASHeader);
my_defstruct(DataSection);

DataSection *datas_create(Database *database);

void datas_alter_bodyoff(Database *database, fileoff_t fileoff, bodyoff_t bodyoff, const void *data, size_t size);
