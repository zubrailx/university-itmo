#pragma once

#include <stdbool.h>

struct dbms;
struct dto_row;

bool row_insert(struct dbms *dbms, const char *table_name, struct dto_row *row);
