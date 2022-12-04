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

// // SLOT PAGE
// fileoff_t dbms_sp_create_close(dbms *dbms, pageoff_t size);
// fileoff_t dbms_sp_create(dbms *dbms, pageoff_t size, slot_page **sp_ptr_out);
// slot_page *dbms_sp_select(dbms *dbms, fileoff_t page_start);
// void dbms_sp_close(slot_page **page_ptr, fileoff_t page_start, dbms *dbms);

// void dbms_sp_insert_data(const void *data, size_t size, dbms *dbms,
//                          fileoff_t *fileoff_out, pageoff_t *pageoff_out);
