#pragma once

struct tpt_column_base;
struct dpt_column;
struct dbms;

const char *tp_column_to_str(const struct tpt_column_base *tpt_base,
                             const struct dpt_column *dpt_column, struct dbms *dbms);
