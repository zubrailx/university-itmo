#pragma once

struct dbms;
struct tp_tuple;
struct dp_tuple;
struct tpt_col_info;

void print_database_tables(struct dbms *dbms);
void print_free_pages(struct dbms *dbms);

void print_table_tuple(const struct tp_tuple *tuple, const struct dp_tuple *dpt,
                       const struct tpt_col_info *col_info, struct dbms *dbms);
void print_table_rows(struct dbms *dbms, const char *table_name);
