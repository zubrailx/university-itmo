#pragma once

#include <stdbool.h>

struct dbms;
struct dto_table;

bool dbms_table_exists(struct dbms *dbms, struct dto_table *table);
bool dbms_create_table(struct dbms *dbms, struct dto_table *table);
bool dbms_drop_table(struct dto_table *table);
