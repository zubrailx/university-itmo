#include "datadistr.h"
#include "core/dbmeta.h"
#include "core/dbms.h"
#include "page.h"

void dbms_dd_create_close(struct dbms *dbms) {
  meta *meta = dbms->meta;
  for (size_t i = 0; i < meta->da_len; ++i) {
    meta->da_arr[i].last =
        dbms_container_create_close(dbms, FILEOFF_NULL, SIZE_DEFAULT);
  }
}

// returns 0 if current page is larger than 
static size_t find_index(size_t entry_size, struct slot_page_entry entries[]) {}

void dbms_dd_insert_data(struct dbms *dbms, void *data, size_t size) {}
