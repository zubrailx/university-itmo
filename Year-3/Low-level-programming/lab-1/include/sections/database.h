#pragma once

#include "sections/base.h"

extern sectoff_t DATABASE_SECTION_SIZE;

// DatabaseSection
struct DSHeader {
	struct BaseSection base_section;
	fileoff_t next;
	fileoff_t previous;
	/* Non-inclusive body offset */
	bodyoff_t index_last;
	/* From start to end */
	bodyoff_t typle_start;
};

struct DatabaseSection {
	struct DSHeader header;
	char body[];
};

struct DSIndex {
	bodyoff_t start;
	bodyoff_t end;
	bool is_cleared;
};

enum TableColumnTypes {
	COLUMN_TYPE_INT = 0,
	COLUMN_TYPE_FLOAT = 1,
	COLUMN_TYPE_STRING = 2,
	COLUMN_TYPE_BOOL = 3,
};

// DATABASE SECTION TABLE TYPLE
// Small string optimizations
struct NameNotIn {
	char str_size[7];
	struct SOPointer ptr;
} __attribute__((packed));

struct SSOName {
	bool is_inline;
	union {
		struct NameNotIn noin;
		char name[sizeof(struct NameNotIn)];
	} u;
} __attribute__((packed));

// small string optimizations
struct DSTHeader {
	fileoff_t fileoff;
	size_t cols;
	struct SSOName name;
	// TableColumns
};

struct DSTColumnLimits {
	bool is_null;
	bool is_unique;
};

struct DSTColumn {
	/* TableColumnType */
	int8_t type;
	struct DSTColumnLimits limits;
	struct SSOName name;
};

struct DSTyple {
	struct DSTHeader header;
	struct DSTColumn columns[];
};

// TO STORE IN RAM
struct DSTHeaderRAM {
	fileoff_t fileoff;// offset to table

	bool so_valid;// does name in file is pointed
	SOPointer so;

	size_t size;
	char *name;
};

struct DSTColumnRAM {
	struct DSTColumnRAM *next;
	/* TableColumnType */
	int8_t type;
	struct DSTColumnLimits limits;

	bool so_valid;// does name in file is pointed
	SOPointer so;

	size_t size;
	char *name;
};

struct DSTypleRAM {
	struct DSTHeaderRAM header;
	size_t cols;
	struct DSTColumnRAM *columns;
};

// HELPER
struct DatabaseSectionWr {// wrapper
	fileoff_t fileoff;
	struct DatabaseSection *ds;
};

struct DSTypleRAMWr {
	fileoff_t ifileoff;// section where index is located
	bodyoff_t ibodyoff;// offset to index
	struct DSTypleRAM *typle;
};

// Typedefs
my_defstruct(DSHeader);
my_defstruct(DatabaseSection);

my_defstruct(DSIndex);

my_defstruct(DSTHeader);
my_defstruct(DSTColumnLimits);
my_defstruct(DSTColumn);
my_defstruct(DSTyple);

my_defstruct(DSTIHeader);
my_defstruct(DSTIColumn);
my_defstruct(DSTypleIn);

my_defstruct(DatabaseSectionWr);

my_defstruct(DSTHeaderRAM);
my_defstruct(DSTColumnRAM);
my_defstruct(DSTypleRAM);
my_defstruct(DSTypleRAMWr);

// Function declarations
size_t ds_get_space_left(const DatabaseSection *dbs);
size_t ds_get_body_size(const DatabaseSection *dbs);
bodyoff_t ds_get_bodyoff(sectoff_t sectoff);
sectoff_t ds_get_sectoff(bodyoff_t bodyoff);

// DatabaseSection specific
DatabaseSectionWr ds_create(Database *database, DatabaseSection *previous,
														fileoff_t previous_pos);
void ds_drop(Database *database, fileoff_t pos);
void ds_alter_bodyoff(Database *database, const void *data, fileoff_t fileoff,
											bodyoff_t offset, size_t size);

DatabaseSectionWr ds_load_next(Database *database, const DatabaseSection *current);

// DatabaseSection Typle and Index specific
// void ds_typle_inline_unload(DSTypleIn **dttyple);
// DSTypleIn *typle_inline_load(Database *database, DatabaseSection *section,
// 														 const DSIndex *index);

DSTypleRAM ds_typle_ram_create(char *name, size_t nsize, DSTColumnRAM *column,
															 size_t cols);
DSTColumnRAM ds_typle_column_ram_create(int8_t type, DSTColumnLimits limits, char *name,
																				size_t size);

DSTypleRAM *ds_typle_to_typle_ram(const DSTyple *typle);
DSTyple *ds_typle_ram_to_typle(const DSTypleRAM *typle);

bool ds_table_create(Database *database, DSTypleRAMWr *wrapper);
DSTypleRAMWr ds_table_select(Database *database, const char *name);
DSTypleRAMWr ds_table_drop(Database *database, const char *name);
