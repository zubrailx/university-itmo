#include "sections/data.h"

sectoff_t DATABASE_SECTION_SIZE = 2048;

static void da_init(DataSection *data) {
	DaSHeader *header = &data->header;
	header->base_section.type = TYPE_DATA;
	header->base_section.size = DATABASE_SECTION_SIZE;
	header->stored_last = 0;
}

DataSectionWr da_create(Database *database) {
	DataSectionWr wrapper;
	wrapper.da = section_malloc(DATABASE_SECTION_SIZE);
	da_init(wrapper.da);
	// next position in file
	wrapper.fileoff = section_create(database, (BaseSection *)&wrapper.da->header);
	return wrapper;
}

bool da_alter(Database *database, fileoff_t fileoff, DataSection *da) {

}
