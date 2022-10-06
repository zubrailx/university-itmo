#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

sectoff_t DATABASE_SECTION_SIZE = 1024;

inline size_t ds_body_size(const DatabaseSection *dbs) {
	return dbs->header.base_section.size - sizeof(DatabaseHeader);
}

inline bodyoff_t ds_get_bodyoff(sectoff_t sectoff) {
	return sectoff - sizeof(DatabaseHeader);
}
inline sectoff_t ds_get_sectoff(bodyoff_t bodyoff) {
	return bodyoff + sizeof(DatabaseHeader);
}

static inline DatabaseSection *section_convert_ds(void *section) {
	return (DatabaseSection *)section;
}

// memory allocation
static DatabaseSection *ds_malloc(const sectoff_t sect_size) {
	void *section = section_malloc(sect_size);
	return section_convert_ds(section);
}

static void ds_init(DatabaseSection *ds) {
	DatabaseHeader *dh = &ds->header;
	// Init database section
	dh->base_section.type = TYPE_DATABASE;
	dh->base_section.size = DATABASE_SECTION_SIZE;
	dh->next = SECTION_OFFSET_NULL;
	dh->previous = SECTION_OFFSET_NULL;
	dh->index_last = 0;
	// TODO: idk nice offset stupid dogshit
	dh->typle_start = ds_get_bodyoff(DATABASE_SECTION_SIZE);
}

inline size_t ds_get_space_left(const DatabaseSection *dbs) {
	return dbs->header.typle_start - dbs->header.index_last;
}

// load + store in file
DatabaseSectionWrapper ds_create(Database *database, DatabaseSection *previous,
																 fileoff_t previous_pos) {
	FILE *file = database->file;
	DatabaseSection *ds = ds_malloc(DATABASE_SECTION_SIZE);
	ds_init(ds);
	// calculate next position in file and write
	fileoff_t next_pos = database->dst.pos_empty;
	fseek(file, (long)next_pos, SEEK_SET);
	fwrite(&ds->header, sizeof(ds->header), 1, database->file);
	fwrite(ds->body, ds_body_size(ds), 1, database->file);

	if (previous != SECTION_OFFSET_NULL) {
		previous->header.next = next_pos;
		fseek(file, (long)(previous_pos + offsetof(DatabaseHeader, next)), SEEK_SET);
		fwrite(&previous->header.next, sizeof(previous->header.next), 1, file);
	}
	database->dst.pos_empty += ds->header.base_section.size;
	return (DatabaseSectionWrapper){.ds = ds, .fileoff = next_pos};
}

void ds_drop(Database *database, fileoff_t pos) {
	FILE *file = database->file;
	DatabaseHeader current;
	// set current section type dumped
	fseek(file, (long)pos, SEEK_SET);
	assert(fread(&current, sizeof(current), 1, file));
	current.base_section.type = TYPE_DUMPED;
	fwrite(&current, sizeof(current), 1, file);
	// update relatives
	if (current.next != SECTION_OFFSET_NULL) {
		fseek(file, (long)current.next, SEEK_SET);
		fwrite(&current.previous, sizeof(current.previous), 1, file);
	}
	if (current.previous != SECTION_OFFSET_NULL) {
		fseek(file, (long)current.previous, SEEK_SET);
		fwrite(&current.next, sizeof(current.next), 1, file);
	}
}

void ds_alter_sectoff(Database *database, const void *data, fileoff_t fileoff,
											 sectoff_t offset, size_t size) {
	assert(database->is_opened);
	FILE *file = database->file;
	fseek(file, (long)fileoff + (long)offset, SEEK_SET);
	fwrite(data, size, 1, file);
}

void ds_alter_bodyoff(Database *database, const void *data, fileoff_t fileoff,
											 bodyoff_t offset, size_t size) {
	ds_alter_sectoff(database, data, fileoff, ds_get_sectoff(offset), size);
}

// NULLABLE
DatabaseSectionWrapper ds_load_next(Database *database,
																		const DatabaseSection *current) {
	FILE *file = database->file;
	fileoff_t pos = current->header.next;
	if (pos == SECTION_OFFSET_NULL) {
		return (DatabaseSectionWrapper){.fileoff = SECTION_OFFSET_NULL, NULL};
	}
	fseek(file, (long)pos, SEEK_SET);
	return (DatabaseSectionWrapper){.fileoff = pos, .ds = ds_load(database, pos)};
}

DatabaseSection *ds_load(Database *database, const fileoff_t offset) {
	void *section = section_load(database, offset);
	return section_convert_ds(section);
}

void ds_unload(DatabaseSection **dbs_ptr) { section_unload((void **)dbs_ptr); }
