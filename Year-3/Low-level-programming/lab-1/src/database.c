#include "sections/database.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "util.h"

sectoff_t DATABASE_SECTION_SIZE = 1024;

inline size_t ds_get_space_left(const DatabaseSection *dbs) {
	return dbs->header.typle_start - dbs->header.index_last;
}

inline size_t ds_body_size(const DatabaseSection *dbs) {
	return dbs->header.base.size - sizeof(DSHeader);
}

inline bodyoff_t ds_get_bodyoff(sectoff_t sectoff) {
	return sectoff - sizeof(DSHeader);
}
inline sectoff_t ds_get_sectoff(bodyoff_t bodyoff) {
	return bodyoff + sizeof(DSHeader);
}

static void ds_init(DatabaseSection *ds, fileoff_t previous) {
	DSHeader *dh = &ds->header;
	// Init database section
	dh->base.type = TYPE_DATABASE;
	dh->base.size = DATABASE_SECTION_SIZE;
	dh->next = SECTION_OFFSET_NULL;
	dh->previous = previous;
	dh->index_last = 0;
	dh->typle_start = ds_get_bodyoff(DATABASE_SECTION_SIZE);
}

// load + store in file
DatabaseSectionWr ds_create(Database *database, DatabaseSection *previous,
														fileoff_t previous_pos) {
	DatabaseSection *ds = (DatabaseSection *)section_malloc(DATABASE_SECTION_SIZE);
	ds_init(ds, SECTION_OFFSET_NULL);
	fileoff_t next_pos = section_create(database, (BaseSection *)ds);
	if (previous != SECTION_OFFSET_NULL) {
		previous->header.next = next_pos;
		section_alter_sectoff(database, previous_pos, offsetof(DSHeader, next),
													&previous->header.next, sizeof(previous->header.next));
	}
	return (DatabaseSectionWr){.ds = ds, .fileoff = next_pos};
}

void ds_alter(Database *database, const fileoff_t fileoff, const DatabaseSection *ds) {
	section_alter(database, fileoff, (BaseSection *)ds);
}

void ds_alter_bodyoff(Database *database, const void *data, fileoff_t fileoff,
											bodyoff_t offset, size_t size) {
	section_alter_sectoff(database, fileoff, ds_get_sectoff(offset), data, size);
}

void ds_drop(Database *database, fileoff_t fileoff) {
	DSHeader *curr = (DSHeader *)section_header_load(database, fileoff, sizeof(DSHeader));
	section_sync_drop(database, fileoff, (BaseSection *)curr);
	// update relatives
	if (curr->next != SECTION_OFFSET_NULL) {
		section_alter_sectoff(database, curr->next, offsetof(DSHeader, previous),
													&curr->previous, sizeof(curr->previous));
	}
	if (curr->previous != SECTION_OFFSET_NULL) {
		section_alter_sectoff(database, curr->previous, offsetof(DSHeader, next),
													&curr->next, sizeof(curr->next));
	}
}

// NULLABLE
DatabaseSectionWr ds_load_next(Database *database, const DatabaseSection *current) {
	fileoff_t pos = current->header.next;
	if (pos == SECTION_OFFSET_NULL) {
		return (DatabaseSectionWr){.fileoff = SECTION_OFFSET_NULL, NULL};
	}
	return (DatabaseSectionWr){.fileoff = pos,
														 .ds = (DatabaseSection *)section_load(database, pos)};
}

DatabaseSection *ds_load(Database *database, fileoff_t fileoff) {
	return (DatabaseSection *)section_load_type(database, fileoff, TYPE_DATABASE);
}

void ds_unload(DatabaseSection **ds) { section_unload((BaseSection **)ds); }
