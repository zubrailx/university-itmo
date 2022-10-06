#pragma once

#include "sections/base.h"

typedef struct DatabaseHeader DatabaseHeader;
typedef struct DatabaseSection DatabaseSection;

typedef struct DatabaseTableIndex DTIndex;

typedef struct DatabaseTableTypleColumn DTTColumn;

typedef struct DatabaseTableTypleHeader DTTHeader;
typedef struct DatabaseTableTypleColumnLimits DTTColumnLimits;
typedef struct DatabaseTableTyple DTTyple;

typedef struct DatabaseTableTypleInlineHeader DTTInlineHeader;
typedef struct DatabaseTableTypleInlineColumn DTTInlineColumn;
typedef struct DatabaseTableTypleInline DTTypleInline;

typedef struct DatabaseSectionWrapper DatabaseSectionWrapper;
typedef struct DatabaseTableTypleIndexWrapper DTTIWrapper;

extern sectoff_t DATABASE_SECTION_SIZE;

// DatabaseSection
struct DatabaseHeader {
	BaseSection base_section;
	fileoff_t next;
	fileoff_t previous;
	/* Non-inclusive body offset */
	bodyoff_t index_last;
	/* From start to end */
	bodyoff_t typle_start;
};

struct DatabaseSection {
	DatabaseHeader header;
	char body[];
};

struct DatabaseTableIndex {
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
struct DatabaseTableTypleHeader {
	fileoff_t fileoff;
	bool is_inline;
	SOPointer str_or_ptr[];
	// TableColumns
};

struct DatabaseTableTypleColumnLimits {
	bool is_null;
	bool is_unique;
};

struct DatabaseTableTypleColumn {
	/* TableColumnType */
	int8_t type;
	// special for
	struct DatabaseTableTypleColumnLimits limits;
	// name of column
	bool is_inline;
	SOPointer str_or_ptr[];// depends on is_inline
};

struct DatabaseTableTyple {
	struct DatabaseTableTypleHeader header;
	// TableColumns
};

// TO STORE IN RAM
struct DatabaseTableTypleInlineHeader {
	fileoff_t fileoff;
	char name[];
};

struct DatabaseTableTypleInlineColumn {
	/* TableColumnType */
	int8_t type;
	// special for
	struct DatabaseTableTypleColumnLimits limits;
	// name of column
	char name[];
};

struct DatabaseTableTypleInline {
	struct DatabaseTableTypleInlineHeader header;
	// TableColumnsInline[]
};

// HELPER
struct DatabaseSectionWrapper {
	fileoff_t fileoff;
	struct DatabaseSection *ds;
};

struct DatabaseTableTypleIndexWrapper {
	bool has_performed;// was operation performed properly
	fileoff_t fileoff; // section where index is located
	bodyoff_t bodyoff; // offset to index
	struct DatabaseTableTypleInline *ityple;
};

size_t ds_get_space_left(const DatabaseSection *dbs);

void dt_ityple_unload(DTTypleInline **dttyple);
DTTypleInline *dt_ityple_load(Database *database, DatabaseSection *section,
															const DTIndex *index);

DTTIWrapper dt_create(Database *database, const fileoff_t address,
											DTTypleInline *ityple, size_t ityple_size);
DTTIWrapper dt_select(Database *database, const char *name);
DTTIWrapper dt_delete(Database *database, const char *name);

size_t ds_get_body_size(const DatabaseSection *dbs);
bodyoff_t ds_get_bodyoff(sectoff_t sectoff);
sectoff_t ds_get_sectoff(bodyoff_t bodyoff);

DatabaseSectionWrapper ds_create(Database *database, DatabaseSection *previous,
																 fileoff_t previous_pos);
void ds_delete(Database *database, fileoff_t pos);
void ds_insert_sectoff(Database *database, const void *data, fileoff_t fileoff,
											 sectoff_t offset, size_t size);
void ds_insert_bodyoff(Database *database, const void *data, fileoff_t fileoff,
											 sectoff_t offset, size_t size);

DatabaseSectionWrapper ds_load_next(Database *database, const DatabaseSection *current);
DatabaseSection *ds_load(Database *database, const fileoff_t offset);
void ds_unload(DatabaseSection **dbs_ptr);
