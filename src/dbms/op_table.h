#pragma once

struct dbms;
struct dp_tuple;
struct dto_row_list;

int dbms_insert_row_list(struct dp_tuple *dpt, const struct dto_row_list *list,
                         struct dbms *dbms);
