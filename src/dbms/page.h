#pragma once

#include <util/internals.h>

#include "core/dbms.h"
#include "io/page/data.h"
#include "io/page/database.h"

// DATABASE PAGE
fileoff_t dbms_dp_create_close(dbms *dbms, pageoff_t size);
fileoff_t dbms_dp_create(dbms *dbms, pageoff_t dp_size, database_page **dp_ptr_out);
database_page *dbms_dp_select(dbms *dbms, fileoff_t page_start);
void dbms_dp_close(database_page **page_ptr, fileoff_t page_start, dbms *dbms);

// DATA PAGE
fileoff_t dbms_da_create_close(dbms *dbms, pageoff_t size);
fileoff_t dbms_da_create(dbms *dbms, pageoff_t size, data_page **da_ptr_out);
data_page *dbms_da_select(dbms *dbms, fileoff_t page_start);
void dbms_da_close(data_page **page_ptr, fileoff_t page_start, dbms *dbms);

void dbms_da_insert_data(const void *data, size_t size, dbms *dbms,
                         fileoff_t *fileoff_out, pageoff_t *pageoff_out);
