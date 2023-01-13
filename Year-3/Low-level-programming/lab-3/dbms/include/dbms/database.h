#pragma once

#include <stdbool.h>


struct dbms *database_open(const char *fname, bool do_trunc);
void database_close(struct dbms **dbms, bool do_remove);
