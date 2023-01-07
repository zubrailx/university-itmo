#pragma once

#include <dbms/dto_table.h>
#include <dbms/util/internals.h>

struct dbms;
struct dp_tuple;
struct database_page;

// Just insert table, no checks for existance

struct dp_tuple *dbms_select_tuple(fileoff_t fileoff, pageoff_t idx_pageoff,
                                   struct dbms *dbms, struct database_page **page_out);

void dbms_create_table(const struct dto_table *dto_table, struct dbms *dbms);
bool dbms_drop_table(fileoff_t fileoff, pageoff_t idx_pageoff, struct dbms *dbms);
bool dbms_find_table(const char *table_name, struct dbms *dbms, fileoff_t *fileoff_out,
                     pageoff_t *pageoff_out);
