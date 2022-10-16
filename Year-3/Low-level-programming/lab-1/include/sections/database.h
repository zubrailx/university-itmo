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

// TO STORE INSIDE FILE AND RAM
struct DSTHeader {
	fileoff_t fileoff;
	bool is_inline;
	struct SOPointer str_or_ptr[];
	// TableColumns
};

struct DSTColumnLimits {
	bool is_null;
	bool is_unique;
};

struct DSTColumn {
	/* TableColumnType */
	int8_t type;
	// special for
	struct DSTColumnLimits limits;
	// name of column
	bool is_inline;
	struct SOPointer str_or_ptr[];// depends on is_inline
};

struct DSTyple {
	struct DSTHeader header;
	// TableColumns
};

// TO STORE IN RAM
struct DSTIHeader {
	fileoff_t fileoff;
	char name[];
};

struct DSTIColumn {
	/* TableColumnType */
	int8_t type;
	// special for
	struct DSTColumnLimits limits;
	// name of column
	char name[];
};

struct DSTypleIn {
	struct DSTIHeader header;
	// TableColumnsInline[]
};

// HELPER
struct DatabaseSectionWr {// wrapper
	fileoff_t fileoff;
	struct DatabaseSection *ds;
};

struct DSTypleInWr {
	bool has_performed;// was operation performed properly
	fileoff_t fileoff; // section where index is located
	bodyoff_t bodyoff; // offset to index
	struct DSTypleIn *ityple;
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
my_defstruct(DSTypleInWr);

// Function declarations
size_t ds_get_space_left(const DatabaseSection *dbs);
size_t ds_get_body_size(const DatabaseSection *dbs);
bodyoff_t ds_get_bodyoff(sectoff_t sectoff);
sectoff_t ds_get_sectoff(bodyoff_t bodyoff);

void typle_inline_unload(DSTypleIn **dttyple);
DSTypleIn *typle_inline_load(Database *database, DatabaseSection *section,
														 const DSIndex *index);

DSTypleInWr ds_table_create(Database *database, const fileoff_t address,
														DSTypleIn *ityple, size_t ityple_size);
DSTypleInWr ds_table_select(Database *database, const char *name);
DSTypleInWr ds_table_drop(Database *database, const char *name);

DatabaseSectionWr ds_create(Database *database, DatabaseSection *previous,
														fileoff_t previous_pos);
void ds_drop(Database *database, fileoff_t pos);
void ds_alter_sectoff(Database *database, const void *data, fileoff_t fileoff,
											sectoff_t offset, size_t size);
void ds_alter_bodyoff(Database *database, const void *data, fileoff_t fileoff,
											sectoff_t offset, size_t size);

DatabaseSectionWr ds_load_next(Database *database, const DatabaseSection *current);
DatabaseSection *ds_load(Database *database, const fileoff_t offset);
void ds_unload(DatabaseSection **dbs_ptr);
