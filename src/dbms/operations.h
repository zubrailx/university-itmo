#pragma once

#include "dto/table.h"
#include <string.h>

struct dbms;

// Just insert table, no checks for existance
void dbms_insert_table(const struct dto_table *dto_table, struct dbms *dbms);
