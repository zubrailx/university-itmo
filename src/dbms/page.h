#pragma once

#include "core/dbms.h"
#include "io/page/p_data.h"
#include "io/page/p_database.h"
#include "io/page/p_table.h"
#include <util/internals.h>

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

// TABLE PAGE
fileoff_t dbms_tp_create_close(dbms *dbms, pageoff_t size, fileoff_t prev_pos);
fileoff_t dbms_tp_create(dbms *dbms, pageoff_t size, fileoff_t prev_pos,
                         table_page **tp_ptr_out);
table_page *dbms_tp_select(dbms *dbms, fileoff_t page_start);
void dbms_tp_close(table_page **page_ptr, fileoff_t page_start, dbms *dbms);
