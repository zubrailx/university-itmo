#include "sections/database.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// typedef struct PairStrSize {
// 	char *str;
// 	size_t size;
// } PairStrSize;

// void typle_inline_unload(DSTypleIn **dttyple) {
// 	free(*dttyple);
// 	*dttyple = NULL;
// }

// Return pair string size and string
// load string from current section or from pointing too
// static PairStrSize str_noinline_load(const Database *database,
// 																		 const DatabaseSection *section,
// 																		 const SOPointer *ptr) {
// 	fileoff_t address = ptr->sect_address;
// 	sectoff_t offset = ptr->offset;
// 	char *str;
// 	size_t size;
// 	if (address == SECTION_CURRENT_PTR) {
// 		size = strlen((const char *)section + offset) + 1;
// 		str = strdup((const char *)section + offset);
// 	} else {
// 		void *loaded_section = section_load(database, address);
// 		size = strlen(loaded_section + offset) + 1;
// 		str = strdup(loaded_section + offset);
// 		section_unload(&loaded_section);
// 	}
// 	return (PairStrSize){.str = str, .size = size};
// }

// DSTypleIn *typle_inline_load(Database *database, DatabaseSection *section,
// 														 const DSIndex *index) {
// 	bodyoff_t off_start = index->start;
// 	const bodyoff_t off_end = index->end;
// 	const DSTyple *typle = (DSTyple *)(section->body + off_start);
// 	// Calculate total size with inlined strings
// 	size_t typle_size = 0;
// 	char *table_name = NULL;
// 	// Calculate only for header
// 	if (typle->header.is_inline) {
// 		// WARN: idk what in brackets is
// 		typle_size += strlen((const char *)typle->header.str_or_ptr) + 1;
// 		table_name = strdup((const char *)typle->header.str_or_ptr);
// 	} else {
// 		const SOPointer *ptr = typle->header.str_or_ptr;
// 		PairStrSize pss = str_noinline_load(database, section, ptr);
// 		typle_size += pss.size;
// 		table_name = pss.str;
// 	}
// 	off_start += sizeof(typle->header) + typle_size;
// 	bodyoff_t off_col_start = off_start;
// 	typle_size += sizeof(DSTIHeader);
// 	// Calculate for columns
// 	while (off_start < off_end) {
// 		DSTColumn *column = (DSTColumn *)(section->body + off_start);
// 		size_t column_size = 0;
// 		if (column->is_inline) {
// 			column_size += strlen((const char *)column->str_or_ptr) + 1;
// 		} else {
// 			SOPointer *ptr = column->str_or_ptr;
// 			PairStrSize pss = str_noinline_load(database, section, ptr);
// 			free(pss.str);
// 			column_size += pss.size;
// 		}
// 		off_start += column_size + sizeof(&column);
// 		typle_size += column_size + sizeof(DSTIColumn);
// 	}
// 	// Insert strings inside allocated memory
// 	// Header
// 	DSTypleIn *ityple = malloc(typle_size);
// 	uint32_t ityple_off = 0;
// 	ityple->header.fileoff = typle->header.fileoff;
// 	strcpy(ityple->header.name, table_name);
// 	free(table_name);
// 	ityple_off += sizeof(ityple->header) + strlen(ityple->header.name) + 1;
// 	// Columns
// 	off_start = off_col_start;
// 	while (off_start < off_end) {
// 		DSTColumn *column = (DSTColumn *)(section->body + off_start);
// 		DSTIColumn *icolumn = (DSTIColumn *)((void *)ityple + ityple_off);
// 		icolumn->type = column->type;
// 		icolumn->limits = column->limits;
// 		if (column->is_inline) {
// 			strcpy(icolumn->name, (char *)column->str_or_ptr);
// 			off_start += strlen((char *)column->str_or_ptr) + 1;
// 		} else {
// 			fileoff_t address = column->str_or_ptr->sect_address;
// 			sectoff_t offset = column->str_or_ptr->offset;
// 			if (address == SECTION_CURRENT_PTR) {
// 				strcpy(icolumn->name, (const char *)section + offset);
// 				off_start += strlen((const char *)section + offset);
// 			} else {
// 				void *loaded_section = section_load(database, address);
// 				strcpy(icolumn->name, loaded_section + offset);
// 				off_start += strlen(loaded_section + offset);
// 				section_unload(&loaded_section);
// 			}
// 		}
// 	}
// 	return ityple;
// }

// DSTypleInWr ds_table_select(Database *database, const char *name) {
// 	assert(database->is_opened);
// 	DatabaseSection *ds = ds_load(database, database->dst.ds_first);
// 	fileoff_t fileoff = database->dst.ds_first;
// 	while (ds != NULL) {
// 		bodyoff_t start = 0;
// 		const bodyoff_t end = ds->header.index_last;
// 		while (start < end) {
// 			DSTypleIn *ityple =
// 					typle_inline_load(database, ds, (const DSIndex *)(ds->body + start));
// 			if (strcmp(ityple->header.name, name) == 0) {
// 				return (DSTypleInWr){.fileoff = fileoff,
// 														 .bodyoff = start,
// 														 .ityple = ityple,
// 														 .has_performed = true};
// 			} else {
// 				free(ityple);
// 			}
// 			start += sizeof(DSIndex);
// 		}
// 		DatabaseSectionWr wrapper = ds_load_next(database, ds);
// 		DatabaseSection *next = wrapper.ds;
// 		fileoff = wrapper.fileoff;
// 		ds_unload(&ds);
// 		ds = next;
// 	}
// 	return (DSTypleInWr){.fileoff = SECTION_OFFSET_NULL,
// 											 .bodyoff = 0,
// 											 .ityple = NULL,
// 											 .has_performed = false};
// }

// Update last index from database section
// static DSIndex ds_index_last_alter(Database *database, fileoff_t fileoff,
// 																	 DatabaseSection *ds, size_t ityple_size) {
// 	DSIndex index;
// 	ds->header.index_last += sizeof(DSIndex);
// 	ds->header.typle_start -= ityple_size;
// 	// calculate offset where to write
// 	sectoff_t off_index_last =
// 			offsetof(DatabaseSection, header) + offsetof(DSHeader, index_last);
// 	sectoff_t off_typle_start =
// 			offsetof(DatabaseSection, header) + offsetof(DSHeader, typle_start);

// 	index = (DSIndex){.is_cleared = false,
// 										.start = ds->header.typle_start,
// 										.end = ds->header.typle_start + ityple_size};
// 	// Save index + update header
// 	ds_alter_sectoff(database, &ds->header.index_last, fileoff, off_index_last,
// 									 sizeof(ds->header.index_last));
// 	ds_alter_sectoff(database, &ds->header.typle_start, fileoff, off_typle_start,
// 									 sizeof(ds->header.typle_start));
// 	ds_alter_bodyoff(database, &index, fileoff, index.start, sizeof(index));
// 	return index;
// }

// static void ds_typle_last_alter(Database *database, fileoff_t fileoff,
// 																DatabaseSection *ds, const DSTypleIn *ityple,
// 																size_t ityple_size) {
// 	DSIndex *index = (DSIndex *)(ds->body + ds->header.index_last - sizeof(DSIndex));
// 	memcpy(ds->body + index->start, ityple, ityple_size);
// 	ds_alter_bodyoff(database, ityple, fileoff, index->start, ityple_size);
// }

// database_table is allocated with name and columns
// Return NULL is exists
inline DSTColumnRAM ds_typle_column_ram_create(const int8_t type,
																							 const DSTColumnLimits limits, char *name,
																							 const size_t size) {
	return (DSTColumnRAM){
			.next = NULL,
			.limits = limits,
			.type = type,
			.size = size,
			.name = name,
			.so_valid = false,
	};
}

inline DSTypleRAM ds_typle_ram_create(char *name, size_t nsize, DSTColumnRAM *column,
																			size_t cols) {
	return (DSTypleRAM){
			.header = (DSTHeaderRAM){.name = name,
															 .size = nsize,
															 .fileoff = SECTION_OFFSET_NULL,
															 .so_valid = false},
			.cols = cols,
			.columns = column,
	};
}

static inline size_t ds_typle_get_size_from_typle_ram(const DSTypleRAM *ram) {
	return sizeof(DSTHeader) + sizeof(DSTColumn) * ram->cols;
}

DSTypleRAM *ds_typle_to_typle_ram(const DSTyple *typle) { return NULL; }

// inline with small string optimizations (doesn't do anything with strings in separate
// sections)
DSTyple *ds_typle_ram_to_typle(const DSTypleRAM *ram) {
	DSTyple *typle = malloc(ds_typle_get_size_from_typle_ram(ram));
	typle->header.cols = ram->cols;
	// Copy header if is not inlined
	if (ram->header.size > sizeof(struct NameNotIn)) {
		typle->header.name.is_inline = false;
		// TODO: test
		memcpy(typle->header.name.u.noin.str_size, (void *)&ram->header.size + 1, 7);
	} else {
		typle->header.name.is_inline = true;
		memcpy(typle->header.name.u.name, ram->header.name, ram->header.size);
	}
	// Set columns
	DSTColumn *colstart = (void *)typle + offsetof(DSTyple, columns);
	DSTColumnRAM *ramcurrent = ram->columns;
	for (int i = 0; i < ram->cols; ++i, ramcurrent = ramcurrent->next) {
		DSTColumn *column = colstart + i;
		column->limits = ramcurrent->limits;
		column->type = ramcurrent->type;
		if (ramcurrent->size > sizeof(struct NameNotIn)) {
			column->name.is_inline = false;
			// TODO: test
			memcpy(column->name.u.noin.str_size, (void *)&ramcurrent->size + 1, 7);
		} else {
			column->name.is_inline = true;
			memcpy(column->name.u.name, ramcurrent->name, ramcurrent->size);
		}
	}
	return typle;
}

static void ds_sync_alter_append_typle(Database *database, fileoff_t ds_fileoff,
																			 DatabaseSection *ds, const DSIndex *index,
																			 const DSTypleRAM *ram) {
	size_t typle_size = ds_typle_get_size_from_typle_ram(ram);
	// Convert ram to typle and find int SOPointers
	DSTyple *typle = ds_typle_ram_to_typle(ram);
	// Find place for not inlined strings and set SOPointer to DSTypleRAM and write when
	// in file
  if (!typle->header.name.is_inline) {
    // find place for them in file and update DSTypleRAM and DSTyple typle

  }
  for (size_t i = 0; i < typle->header.cols; ++i) {
    if (!typle->columns[i].name.is_inline) {
      // do the same
    }
  }
	bodyoff_t offset = ds->header.typle_start - typle_size;
	memcpy(ds->body + offset, typle, 1);
	ds_alter_bodyoff(database, typle, ds_fileoff, offset, typle_size);
}

// Append index to DatabaseSection in RAM and file section with address fileoff
static void ds_sync_alter_append_index(Database *database, fileoff_t ds_fileoff,
																			 DatabaseSection *ds, const DSIndex *index) {
	bodyoff_t ilast = ds->header.index_last;
	memcpy(ds->body + ilast, index, 1);
	ds_alter_bodyoff(database, index, ds_fileoff, ilast, sizeof(DSIndex));
}

// True if it was created, false otherwise
bool ds_table_create(Database *database, DSTypleRAMWr *wrapper) {
	assert(database->is_opened);
	DSTypleRAMWr selected = ds_table_select(database, wrapper->typle->header.name);
	if (selected.typle == NULL) {
		return false;
	}
	size_t typle_size = ds_typle_get_size_from_typle_ram(wrapper->typle);
	// put table in current section or create new
	DatabaseSection *ds = ds_load(database, database->dst.ds_last);
	wrapper->ifileoff = database->dst.ds_last;
	size_t space_left = ds_get_space_left(ds);
	if (space_left <= typle_size + sizeof(DSIndex)) {
		ds_unload(&ds);
		DatabaseSectionWr wrapper = ds_create(database, ds, database->dst.ds_last);
		ds = wrapper.ds;
		assert(ds_get_space_left(ds) <= typle_size + sizeof(DSIndex));
		wrapper.fileoff = wrapper.fileoff;
	}
	DSTyple *typle = create
			// Update returned values
			ds_index_typle_alter(database, wrapper->ifileoff, ds, ityple, ityple_size);
	wrapper->ibodyoff = ds->header.index_last - sizeof(DSIndex);
	ds_unload(&ds);
	return true;
}

// DSTypleInWr ds_table_drop(Database *database, const char *name) {
// 	assert(database->is_opened);
// 	FILE *file = database->file;
// 	DSTypleInWr wrapper = ds_table_select(database, name);
// 	if (wrapper.has_performed == false) {
// 		return wrapper;
// 	}
// 	DatabaseSection *ds = ds_load(database, wrapper.fileoff);
// 	DSIndex *index = (DSIndex *)(ds->body + wrapper.bodyoff);
// 	index->is_cleared = true;
// 	fseek(file, wrapper.fileoff + ds_get_sectoff(wrapper.bodyoff), SEEK_SET);
// 	fwrite(index, sizeof(*index), 1, file);
// 	ds_unload(&ds);
// 	return wrapper;
// }
