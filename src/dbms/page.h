#pragma once

#include "core/dbms.h"
#include "io/page/p_container.h"
#include "io/page/p_database.h"
#include "io/page/p_slot.h"
#include "io/page/p_table.h"
#include <util/internals.h>

// PAGE CONTAINER
fileoff_t dbms_container_create_close(struct dbms *dbms, const fileoff_t prev,
                                      pageoff_t size);
fileoff_t dbms_container_create(struct dbms *dbms, pageoff_t size, const fileoff_t prev,
                                struct page_container **pc_ptr_out);

struct page_container *dbms_container_open(const struct dbms *dbms,
                                           const fileoff_t pc_loc);

void dbms_container_close(struct page_container **page_ptr, fileoff_t page_start,
                          struct dbms *dbms);
bool dbms_container_find(const struct dbms *dbms, const pageoff_t size,
                         fileoff_t *loc_out, pageoff_t *off_out);

// DATABASE PAGE
fileoff_t dbms_dp_create_close(dbms *dbms, pageoff_t size);
fileoff_t dbms_dp_create(dbms *dbms, pageoff_t dp_size, database_page **dp_ptr_out);
database_page *dbms_dp_open(dbms *dbms, fileoff_t page_start);
void dbms_dp_close(database_page **page_ptr, fileoff_t page_start, dbms *dbms);

// // TABLE PAGE
fileoff_t dbms_tp_create_close(dbms *dbms, pageoff_t size, fileoff_t prev_pos,
                               const dp_tuple *tuple);
fileoff_t dbms_tp_create(dbms *dbms, const pageoff_t size, const fileoff_t prev_pos,
                         const dp_tuple *tuple, table_page **tp_ptr_out);
table_page *dbms_tp_open(dbms *dbms, fileoff_t page_start);
void dbms_tp_close(table_page **page_ptr, fileoff_t page_start, dbms *dbms);
