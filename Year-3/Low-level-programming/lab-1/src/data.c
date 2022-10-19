#include "sections/data.h"

#include <assert.h>
#include <stddef.h>

sectoff_t DATA_SECTION_SIZE = 2048;

static void da_init(DataSection *data) {
	DaSHeader *header = &data->header;
	header->base.type = TYPE_DATA;
	header->base.size = DATA_SECTION_SIZE;
	header->stored_last = 0;
}

DataSectionWr da_create(Database *db) {
	DataSectionWr wrapper;
	wrapper.da = (DataSection *)section_malloc(DATA_SECTION_SIZE);
	da_init(wrapper.da);
	wrapper.fileoff = section_create(db, (BaseSection *)&wrapper.da->header);
	return wrapper;
}

void da_alter(Database *db, fileoff_t fileoff, DataSection *da) {
	section_alter(db, fileoff, (BaseSection *)da);
}

void da_alter_bodyoff(Database *db, fileoff_t fileoff, bodyoff_t bodyoff,
											const void *data, size_t size) {
	section_alter_sectoff(db, fileoff, sizeof(DaSHeader) + bodyoff, data, size);
}

SOPointer da_alter_append(Database *db, const void *data, size_t size) {
	DaSHeader *header =
			(DaSHeader *)section_header_load(db, db->dst.da_last, sizeof(DaSHeader));
	assert(header != NULL);
	// Check size of header and create new if false
	if (header->base.size - sizeof(DaSHeader) - header->stored_last < size) {
		section_unload((BaseSection **)&header);
		DataSectionWr wrapper = da_create(db);
		header = &wrapper.da->header;
		assert(header->base.size - sizeof(DaSHeader) > size);
	}
	// Write data and update pointer to empty first
	SOPointer pointer = (SOPointer){.fileoff = db->dst.da_last, header->stored_last};
	da_alter_bodyoff(db, db->dst.da_last, header->stored_last, data, size);
	header->stored_last += size;
	section_alter_sectoff(db, db->dst.da_last, offsetof(DaSHeader, stored_last),
												&header->stored_last, sizeof(header->stored_last));
	// Close resources
	section_unload((BaseSection **)&header);
	return pointer;
}

void da_drop(Database *db, fileoff_t fileoff) { section_drop(db, fileoff); }

DataSection *da_load(Database *db, fileoff_t fileoff) {
	return (DataSection *)section_load_type(db, fileoff, TYPE_DATA);
}
void da_unload(DataSection **ds) { section_unload((BaseSection **)ds); }
