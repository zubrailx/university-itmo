#pragma once

#include <stdbool.h>

struct dbms;
struct dto_row_list;

int row_list_insert(struct dbms *dbms, const char *table_name,
                    struct dto_row_list *list);
