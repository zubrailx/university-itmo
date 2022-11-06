#include "data.h"

#include <assert.h>
#include <stddef.h>

pageoff_t DATA_PAGE_SIZE = 2048;

static void da_init(data_page *data) {
  data_header *header = &data->header;
  header->base.type = TYPE_DATA;
  header->base.size = DATA_PAGE_SIZE;
  header->stored_last = 0;
}

data_page_wr da_create(database *db) {
  data_page_wr wrapper;
  wrapper.da = (data_page *)page_malloc(DATA_PAGE_SIZE);
  da_init(wrapper.da);
  wrapper.fileoff = page_create(db, (base_page *)&wrapper.da->header);
  return wrapper;
}

void da_alter(database *db, fileoff_t fileoff, data_page *da) {
  page_alter(db, fileoff, (base_page *)da);
}

void da_alter_bodyoff(database *db, fileoff_t fileoff, bodyoff_t bodyoff,
                      const void *data, size_t size) {
  page_alter_pageoff(db, fileoff, sizeof(data_header) + bodyoff, data, size);
}

po_ptr da_alter_append(database *db, const void *data, size_t size) {
  data_header *header =
      (data_header *)page_header_load(db, db->dst.da.last, sizeof(data_header));
  assert(header != NULL);
  // Check size of header and create new if false
  if (header->base.size - sizeof(data_header) - header->stored_last < size) {
    page_unload((base_page **)&header);
    data_page_wr wrapper = da_create(db);
    header = &wrapper.da->header;
    assert(header->base.size - sizeof(data_header) > size);
  }
  // Write data and update pointer to empty first
  po_ptr pointer = (po_ptr){.fileoff = db->dst.da.last, header->stored_last};
  da_alter_bodyoff(db, db->dst.da.last, header->stored_last, data, size);
  header->stored_last += size;
  page_alter_pageoff(db, db->dst.da.last, offsetof(data_header, stored_last),
                     &header->stored_last, sizeof(header->stored_last));
  // Close resources
  page_unload((base_page **)&header);
  return pointer;
}

void da_drop(database *db, fileoff_t fileoff) { page_drop(db, fileoff); }

data_page *da_load(database *db, fileoff_t fileoff) {
  return (data_page *)page_load_check_type(db, fileoff, TYPE_DATA);
}
void da_unload(data_page **ds) { page_unload((base_page **)ds); }
