#pragma once

#include "core/dbms.h"
#include "io/page/database.h"
#include <util/internals.h>

fileoff_t dbms_dp_create(dbms *dbms);
fileoff_t dbms_da_create(dbms *dbms);

