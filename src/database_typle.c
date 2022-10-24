#include "sections/database.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "sections/data.h"
#include "util.h"

// inits string size
static void typle_ram_header_init(DSTHeaderRAM *ramh, const DSTHeader *header) {
	ramh->changed = false;
	ramh->cols = header->cols;
	ramh->fileoff = header->fileoff;
	ramh->is_inline = header->name.is_inline;
	ramh->so = header->name.u.noin.ptr;
	size_to_sso(strlen(ramh->name), ramh->ssize);
}

static void typle_ram_column_init(DSTColumnRAM *ramc, DSTColumnRAM *next,
																	const DSTColumn *column) {
	ramc->next = next;
	ramc->type = column->type;
	ramc->limits = column->limits;
	ramc->changed = false;
	ramc->is_inline = column->name.is_inline;
	ramc->so = column->name.u.noin.ptr;
	size_to_sso(strlen(ramc->name), ramc->ssize);
}

static void typle_ram_header_load(DSTHeaderRAM *ramh, Database *db,
																	const DSTyple *typle) {
	// Header: init fields and copy string
	if (typle->header.name.is_inline) {
		// also puts null terminator
		ramh->name = strndup(typle->header.name.u.name, SSO_MXLEN);
	} else {
		StrNoIn noin = typle->header.name.u.noin;
		DataSection *da = da_load(db, noin.ptr.fileoff);
		assert(da != NULL);
		ramh->name = strndup((void *)da + noin.ptr.offset, sso_to_size(noin.ssize));
		da_unload(&da);
	}
	typle_ram_header_init(ramh, &typle->header);
}

static void typle_ram_header_unload(DSTHeaderRAM *const ramh) { free(ramh->name); }

static DSTColumnRAM *typle_ram_columns_load(Database *db, const DSTyple *typle) {
	DSTColumn column;
	DSTColumnRAM *ramc = NULL, *ram_next = NULL;
	for (size_t i = 0; i < typle->header.cols; ++i) {
		size_t irev = typle->header.cols - 1 - i;
		column = typle->columns[irev];
		ramc = my_malloc(DSTColumnRAM);
		if (column.name.is_inline) {
			ramc->name = strndup(column.name.u.name, SSO_MXLEN);
		} else {
			StrNoIn noin = column.name.u.noin;
			DataSection *da = da_load(db, noin.ptr.fileoff);
			assert(da != NULL);
			ramc->name = strndup((void *)da + noin.ptr.offset, sso_to_size(noin.ssize));
			da_unload(&da);
		}
		typle_ram_column_init(ramc, ram_next, &column);
		ram_next = ramc;
	}
	return ram_next;// as we started from end, we need to return last occurence
}

static void typle_ram_columns_unload(DSTColumnRAM **const col_ptr) {
	DSTColumnRAM *col = *col_ptr;
	DSTColumnRAM *prev;
	while (col != NULL) {
		prev = col;
		col = col->next;
		free(prev->name);
		free(prev);
	}
	*col_ptr = NULL;
}

static inline size_t typle_size_from_typle_ram(const DSTypleRAM *ram) {
	return sizeof(DSTHeader) + sizeof(DSTColumn) * ram->header.cols;
}

// no-check for empty space (+ update DatabaseSection)
static DSIndex typle_index_create_sync(Database *db, DatabaseSection *ds,
																			 const fileoff_t fileoff,
																			 const size_t typle_size) {
	DSIndex index = (DSIndex){.is_cleared = false,
														.start = ds->header.typle_start - typle_size,
														.end = ds->header.typle_start};
	// calculate offset where to write
	const sectoff_t off_index_last = offsetof(DSHeader, index_last);
	const sectoff_t off_typle_start = offsetof(DSHeader, typle_start);
	// update ram
	const bodyoff_t index_prev = ds->header.index_last;
	ds->header.index_last += sizeof(DSIndex);
	ds->header.typle_start -= typle_size;
	// alter sync header
	section_alter_sync_sectoff(db, fileoff, off_index_last, (BaseSection *)ds,
														 &ds->header.index_last, sizeof(ds->header.index_last));
	section_alter_sync_sectoff(db, fileoff, off_typle_start, (BaseSection *)ds,
														 &ds->header.typle_start, sizeof(ds->header.typle_start));
	// alter sync index
	ds_alter_sync_bodyoff(db, ds, &index, fileoff, index_prev, sizeof(index));
	return index;
}

// write inlined string and also noninlined (no sense to divide this function)
// also update typleRAM and DatabaseSection
static void typle_alter_sync(Database *db, const fileoff_t fileoff, DatabaseSection *ds,
														 const DSIndex *index, DSTypleRAM *ram) {
	// for each non-inlined cell check, find place, update DSTypleRAM
	if (!ram->header.is_inline && ram->header.changed) {
		ram->header.so =
				da_alter_append(db, ram->header.name, sso_to_size(ram->header.ssize));
		ram->header.changed = false;
	}
	DSTColumnRAM *ramc = ram->columns;
	while (ramc != NULL) {
		if (!ramc->is_inline && ramc->changed) {
			ramc->so = da_alter_append(db, ram->header.name, sso_to_size(ram->header.ssize));
			ramc->changed = false;
		}
		ramc = ramc->next;
	}
	// write new updated typle to file
	DSTyple *typle = ds_typle_ram_to_typle(ram);
	size_t typle_size = typle_size_from_typle_ram(ram);
	ds_alter_sync_bodyoff(db, ds, typle, fileoff, index->start, typle_size);
}

DSTypleRAM *ds_typle_ram_load(Database *db, DatabaseSection *section,
															const DSIndex *index) {
	DSTyple *typle = (DSTyple *)(section->body + index->start);
	DSTypleRAM *ram = my_malloc(DSTypleRAM);
	typle_ram_header_load(&ram->header, db, typle);
	ram->columns = typle_ram_columns_load(db, typle);
	return ram;
}

void ds_typle_ram_unload(DSTypleRAM **const ram_ptr) {
	DSTypleRAM *ram = *ram_ptr;
	typle_ram_header_unload(&ram->header);
	if (ram->columns != NULL) {
		typle_ram_columns_unload(&ram->columns);
	}
	free(ram);
	*ram_ptr = NULL;
}

// database_table is allocated with name and columns
// Return NULL is exists
DSTColumnRAM ds_typle_column_ram_create(const int8_t type, const DSTColumnLimits limits,
																				char *name, const size_t size) {
	DSTColumnRAM column = (DSTColumnRAM){
			.next = NULL,
			.limits = limits,
			.type = type,
			.name = name,
			.changed = true,
	};
	size_to_sso(size, column.ssize);
	// set is_inlined flag for sso
	if (size > SSO_MXLEN) {
		column.is_inline = false;
	}
	return column;
}

DSTypleRAM ds_typle_ram_create(char *name, size_t nsize, DSTColumnRAM *column,
															 size_t cols) {
	DSTypleRAM ram = (DSTypleRAM){
			.header = (DSTHeaderRAM){.name = name,
															 .fileoff = SECTION_OFFSET_NULL,
															 .changed = true,
															 .cols = cols},
			.columns = column,
	};
	size_to_sso(nsize, ram.header.ssize);
	// set is_inlined flag for sso
	if (nsize > SSO_MXLEN) {
		ram.header.is_inline = false;
	}
	return ram;
}

// inline with small string optimizations (doesn't do anything with strings in separate
// sections)
DSTyple *ds_typle_ram_to_typle(const DSTypleRAM *ram) {
	DSTyple *typle = calloc(1, typle_size_from_typle_ram(ram));
	typle->header.cols = ram->header.cols;
	// Copy header if is not inlined
	if (sso_to_size(ram->header.ssize) > sizeof(struct StrNoIn)) {
		typle->header.name.is_inline = false;
		memcpy(typle->header.name.u.noin.ssize, ram->header.ssize, SSO_STRING_SIZE);
	} else {
		typle->header.name.is_inline = true;
		memcpy(typle->header.name.u.name, ram->header.name, sso_to_size(ram->header.ssize));
	}
	// Set columns
	DSTColumn *colstart = (void *)typle + offsetof(DSTyple, columns);
	DSTColumnRAM *ramcurrent = ram->columns;
	for (int i = 0; i < ram->header.cols; ++i, ramcurrent = ramcurrent->next) {
		DSTColumn *column = colstart + i;
		column->limits = ramcurrent->limits;
		column->type = ramcurrent->type;
		if (sso_to_size(ramcurrent->ssize) > sizeof(struct StrNoIn)) {
			column->name.is_inline = false;
			memcpy(column->name.u.noin.ssize, ramcurrent->ssize, SSO_STRING_SIZE);
		} else {
			column->name.is_inline = true;
			memcpy(column->name.u.name, ramcurrent->name, sso_to_size(ramcurrent->ssize));
		}
	}
	return typle;
}

DSTypleRAMWr ds_table_select(Database *db, const char *name) {
	assert(db->is_opened);
	DatabaseSection *ds = ds_load(db, db->dst.ds_first);
	DSTypleRAMWr result = {.typle = NULL};
	fileoff_t fileoff = db->dst.ds_first;
	while (ds != NULL) {
		bodyoff_t start = 0;
		const bodyoff_t end = ds->header.index_last;
		DSTHeaderRAM header;
		while (start < end) {
			DSIndex *index = (DSIndex *)(ds->body + start);
			typle_ram_header_load(&header, db, (DSTyple *)(ds->body + index->start));
			if (strcmp(header.name, name) == 0) {
				result = (DSTypleRAMWr){.ifileoff = fileoff,
																.ibodyoff = start,
																.typle = ds_typle_ram_load(db, ds, index)};
				break;
			}
			typle_ram_header_unload(&header);
			start += sizeof(DSIndex);
		}
		// if result found -> close resources and return result
		if (result.typle != NULL) {
			ds_unload(&ds);
			return result;
		} else {
			DatabaseSectionWr wrapper = ds_load_next(db, ds);
			fileoff = wrapper.fileoff;
			ds_unload(&ds);
			ds = wrapper.ds;
		}
	}
	return (DSTypleRAMWr){.ifileoff = SECTION_OFFSET_NULL, .ibodyoff = 0, .typle = NULL};
}

// if found -> return NULL as DSTypleRAMWr.typle (changed for each is set to true)
DSTypleRAMWr ds_table_create(Database *database, DSTypleRAM *ram) {
	DSTypleRAMWr result = ds_table_select(database, ram->header.name);
	if (result.typle != NULL) {
		ds_typle_ram_unload(&result.typle);
		return result;
	}
	size_t typle_size = typle_size_from_typle_ram(ram);
	// Put table in current section or create new
	DatabaseSection *ds = ds_load(database, database->dst.ds_last);
	result.ifileoff = database->dst.ds_last;
	size_t space_left = ds_get_space_left(ds);
	if (space_left <= typle_size + sizeof(DSIndex)) {
		ds_unload(&ds);
		DatabaseSectionWr wrapper = ds_create(database, ds, database->dst.ds_last);
		ds = wrapper.ds;
		assert(ds_get_space_left(ds) <= typle_size + sizeof(DSIndex));
		result.ifileoff = wrapper.fileoff;
	}
	// Append current section with typle + add notinlined fields
	DSIndex index = typle_index_create_sync(database, ds, result.ifileoff, typle_size);
	typle_alter_sync(database, result.ifileoff, ds, &index, ram);
	result.ibodyoff = ds->header.index_last - sizeof(DSIndex);
	result.typle = ram;
	return result;
}

DSTypleRAMWr ds_table_drop(Database *db, const char *name) {
	DSTypleRAMWr wrapper = ds_table_select(db, name);
	if (wrapper.typle == NULL) {
		return wrapper;
	}
	DSIndex index = {.is_cleared = true};
	ds_alter_bodyoff(db, &index, wrapper.ifileoff, wrapper.ibodyoff, sizeof(index));
	return wrapper;
}
