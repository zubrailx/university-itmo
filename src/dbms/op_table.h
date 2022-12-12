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

// bool dbms_update_row(const struct tp_tuple *new, struct tp_tuple *old,
//                      size_t tuple_size, struct table_page *tp, struct dp_tuple *dpt,
//                      struct tpt_col_info *col_info, struct dbms *dbms);

// bool dbms_remove_row(struct tp_tuple *old, struct table_page *page,
//                      struct dp_tuple *dpt, struct tpt_col_info *col_info,
//                      struct dbms *dbms);
