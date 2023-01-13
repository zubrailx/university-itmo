#pragma once

#include <stdbool.h>

struct dbms;
struct dto_table;
struct dp_tuple;

bool table_exists(struct dbms *dbms, const char *name);
bool table_create(struct dbms *dbms, struct dto_table *table);
bool table_drop(struct dbms *dbms, const char *name);

struct dto_table *table_construct_header(struct dbms *dbms, const char *table_name);
