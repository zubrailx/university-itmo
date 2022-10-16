#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct PairStrSize {
	char *str;
	size_t size;
} PairStrSize;

void typle_inline_unload(DSTypleIn **dttyple) {
	free(*dttyple);
	*dttyple = NULL;
}

// Return pair string size and string
// load string from current section or from pointing too
static PairStrSize str_noinline_load(const Database *database,
																		 const DatabaseSection *section,
																		 const SOPointer *ptr) {
	fileoff_t address = ptr->sect_address;
	sectoff_t offset = ptr->offset;
	char *str;
	size_t size;
	if (address == SECTION_CURRENT_PTR) {
		size = strlen((const char *)section + offset) + 1;
		str = strdup((const char *)section + offset);
	} else {
		void *loaded_section = section_load(database, address);
		size = strlen(loaded_section + offset) + 1;
		str = strdup(loaded_section + offset);
		section_unload(&loaded_section);
	}
	return (PairStrSize){.str = str, .size = size};
}

DSTypleIn *typle_inline_load(Database *database, DatabaseSection *section,
														 const DSIndex *index) {
	bodyoff_t off_start = index->start;
	const bodyoff_t off_end = index->end;
	const DSTyple *typle = (DSTyple *)(section->body + off_start);
	// Calculate total size with inlined strings
	size_t typle_size = 0;
	char *table_name = NULL;
	// Calculate only for header
	if (typle->header.is_inline) {
		// WARN: idk what in brackets is
		typle_size += strlen((const char *)typle->header.str_or_ptr) + 1;
		table_name = strdup((const char *)typle->header.str_or_ptr);
	} else {
		const SOPointer *ptr = typle->header.str_or_ptr;
		PairStrSize pss = str_noinline_load(database, section, ptr);
		typle_size += pss.size;
		table_name = pss.str;
	}
	off_start += sizeof(typle->header) + typle_size;
	bodyoff_t off_col_start = off_start;
	typle_size += sizeof(DSTIHeader);
	// Calculate for columns
	while (off_start < off_end) {
		DSTColumn *column = (DSTColumn *)(section->body + off_start);
		size_t column_size = 0;
		if (column->is_inline) {
			column_size += strlen((const char *)column->str_or_ptr) + 1;
		} else {
			SOPointer *ptr = column->str_or_ptr;
			PairStrSize pss = str_noinline_load(database, section, ptr);
			free(pss.str);
			column_size += pss.size;
		}
		off_start += column_size + sizeof(&column);
		typle_size += column_size + sizeof(DSTIColumn);
	}
	// Insert strings inside allocated memory
	// Header
	DSTypleIn *ityple = malloc(typle_size);
	uint32_t ityple_off = 0;
	ityple->header.fileoff = typle->header.fileoff;
	strcpy(ityple->header.name, table_name);
	free(table_name);
	ityple_off += sizeof(ityple->header) + strlen(ityple->header.name) + 1;
	// Columns
	off_start = off_col_start;
	while (off_start < off_end) {
		DSTColumn *column = (DSTColumn *)(section->body + off_start);
		DSTIColumn *icolumn = (DSTIColumn *)((void *)ityple + ityple_off);
		icolumn->type = column->type;
		icolumn->limits = column->limits;
		if (column->is_inline) {
			strcpy(icolumn->name, (char *)column->str_or_ptr);
			off_start += strlen((char *)column->str_or_ptr) + 1;
		} else {
			fileoff_t address = column->str_or_ptr->sect_address;
			sectoff_t offset = column->str_or_ptr->offset;
			if (address == SECTION_CURRENT_PTR) {
				strcpy(icolumn->name, (const char *)section + offset);
				off_start += strlen((const char *)section + offset);
			} else {
				void *loaded_section = section_load(database, address);
				strcpy(icolumn->name, loaded_section + offset);
				off_start += strlen(loaded_section + offset);
				section_unload(&loaded_section);
			}
		}
	}
	return ityple;
}

DSTypleInWr ds_table_select(Database *database, const char *name) {
	assert(database->is_opened);
	DatabaseSection *ds = ds_load(database, database->dst.ds_first);
	fileoff_t fileoff = database->dst.ds_first;
	while (ds != NULL) {
		bodyoff_t start = 0;
		const bodyoff_t end = ds->header.index_last;
		while (start < end) {
			DSTypleIn *ityple =
					typle_inline_load(database, ds, (const DSIndex *)(ds->body + start));
			if (strcmp(ityple->header.name, name) == 0) {
				return (DSTypleInWr){.fileoff = fileoff,
														 .bodyoff = start,
														 .ityple = ityple,
														 .has_performed = true};
			} else {
				free(ityple);
			}
			start += sizeof(DSIndex);
		}
		DatabaseSectionWr wrapper = ds_load_next(database, ds);
		DatabaseSection *next = wrapper.ds;
		fileoff = wrapper.fileoff;
		ds_unload(&ds);
		ds = next;
	}
	return (DSTypleInWr){.fileoff = SECTION_OFFSET_NULL,
											 .bodyoff = 0,
											 .ityple = NULL,
											 .has_performed = false};
}

// Update last index from database section
static DSIndex ds_index_last_alter(Database *database, fileoff_t fileoff,
																	 DatabaseSection *ds, size_t ityple_size) {
	DSIndex index;
	ds->header.index_last += sizeof(DSIndex);
	ds->header.typle_start -= ityple_size;
	// calculate offset where to write
	sectoff_t off_index_last =
			offsetof(DatabaseSection, header) + offsetof(DSHeader, index_last);
	sectoff_t off_typle_start =
			offsetof(DatabaseSection, header) + offsetof(DSHeader, typle_start);

	index = (DSIndex){.is_cleared = false,
										.start = ds->header.typle_start,
										.end = ds->header.typle_start + ityple_size};
	// Save index + update header
	ds_alter_sectoff(database, &ds->header.index_last, fileoff, off_index_last,
									 sizeof(ds->header.index_last));
	ds_alter_sectoff(database, &ds->header.typle_start, fileoff, off_typle_start,
									 sizeof(ds->header.typle_start));
	ds_alter_bodyoff(database, &index, fileoff, index.start, sizeof(index));
	return index;
}

static void ds_typle_last_alter(Database *database, fileoff_t fileoff,
																DatabaseSection *ds, const DSTypleIn *ityple,
																size_t ityple_size) {
	DSIndex *index = (DSIndex *)(ds->body + ds->header.index_last - sizeof(DSIndex));
	memcpy(ds->body + index->start, ityple, ityple_size);
	ds_alter_bodyoff(database, ityple, fileoff, index->start, ityple_size);
}
// Save DTTypleInline and DTIndex in file and in DatabaseSection * object
static void ds_index_typle_alter(Database *database, const fileoff_t fileoff,
																 DatabaseSection *ds, const DSTypleIn *ityple,
																 const size_t ityple_size) {
	ds_index_last_alter(database, fileoff, ds, ityple_size);
	ds_typle_last_alter(database, fileoff, ds, ityple, ityple_size);
}
// database_table is allocated with name and columns
// Return NULL is exists
DSTypleInWr ds_table_create(Database *database, const fileoff_t address,
														DSTypleIn *ityple, size_t ityple_size) {
	assert(database->is_opened);
	DSTypleInWr selected = ds_table_select(database, ityple->header.name);
	if (selected.ityple != NULL) {
		free(selected.ityple);
		selected.ityple = NULL;
		selected.has_performed = false;
		return selected;
	}

	DSTypleInWr returned;
	DatabaseSection *ds = ds_load(database, database->dst.ds_last);
	// update fileoff
	returned.fileoff = database->dst.ds_last;
	size_t space_left = ds_get_space_left(ds);
	if (space_left <= ityple_size + sizeof(DSIndex)) {
		ds_unload(&ds);
		DatabaseSectionWr wrapper = ds_create(database, ds, database->dst.ds_last);
		ds = wrapper.ds;
		assert(ds_get_space_left(ds) <= ityple_size + sizeof(DSIndex));
		// update fileoff
		returned.fileoff = wrapper.fileoff;
	}
	// Update returned values
	ds_index_typle_alter(database, returned.fileoff, ds, ityple, ityple_size);
	returned.has_performed = true;
	returned.bodyoff = ds->header.index_last - sizeof(DSIndex);
	returned.ityple = ityple;
	ds_unload(&ds);
	return returned;
}

DSTypleInWr ds_table_drop(Database *database, const char *name) {
	assert(database->is_opened);
	FILE *file = database->file;
	DSTypleInWr wrapper = ds_table_select(database, name);
	if (wrapper.has_performed == false) {
		return wrapper;
	}
	DatabaseSection *ds = ds_load(database, wrapper.fileoff);
	DSIndex *index = (DSIndex *)(ds->body + wrapper.bodyoff);
	index->is_cleared = true;
	fseek(file, wrapper.fileoff + ds_get_sectoff(wrapper.bodyoff), SEEK_SET);
	fwrite(index, sizeof(*index), 1, file);
	ds_unload(&ds);
	return wrapper;
}
