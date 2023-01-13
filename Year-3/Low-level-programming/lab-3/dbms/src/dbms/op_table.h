#pragma once

#include <stdbool.h>
#include <stddef.h>

struct dbms;
struct dp_tuple;
struct table_page;
struct tp_tuple;
struct dto_row_list;
struct tpt_col_info;

int dbms_insert_row_list(struct dp_tuple *dpt, const struct dto_row_list *list,
                         struct dbms *dbms);

int dbms_find_column_idx(const struct dp_tuple *dpt, const char *column_name,
                         struct dbms *dbms);
