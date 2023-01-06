#pragma once

#include <stdbool.h>

struct dbms *database_open(const char *fname);
void database_close(struct dbms **dbms, bool do_remove);
