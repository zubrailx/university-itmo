#pragma once

#include <stddef.h>

struct tp_tuple;
struct dp_tuple;
struct tpt_col_info;
struct dbms;

void dbms_update_row_pre(const struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
                         const struct dp_tuple *dpt,
                         const struct tpt_col_info *col_info, struct dbms *dbms);

void dbms_remove_row_pre(const struct tp_tuple *old, const struct dp_tuple *dpt,
                         const struct tpt_col_info *col_info, struct dbms *dbms);
