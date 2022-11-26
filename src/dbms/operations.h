#pragma once

#include "dto/table.h"
#include <string.h>
#include <util/internals.h>

struct dbms;

// Just insert table, no checks for existance
void dbms_create_table(const struct dto_table *dto_table, struct dbms *dbms);
bool dbms_drop_table(const fileoff_t fileoff, const pageoff_t pageoff, struct dbms *dbms);
