#pragma once

#include <string.h>

#include "converters/table.h"
#include "core/dbfile.h"
#include "core/dbms.h"
#include "core/meta.h"
#include "dto/table.h"
#include "io/page/database.h"
#include "page.h"

// Just insert table, no checks for existance
void dbms_insert_table(const struct dto_table *dto_table, struct dbms *dbms);
