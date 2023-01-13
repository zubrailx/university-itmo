#pragma once

struct dp_tuple;
struct dto_table;
struct dbms;

void dp_tuple_to_dto(struct dp_tuple *tuple, struct dto_table **dto_out,
                     struct dbms *dbms);
