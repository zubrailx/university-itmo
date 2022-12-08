#pragma once

struct dbms;
struct dto_row;
struct dp_tuple;

int dbms_insert_row(struct dp_tuple *tuple, const struct dto_row *row,
                    struct dbms *dbms);
