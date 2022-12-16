#pragma once

#include "util/column_types.h"

struct dbms;
struct fast_binop_func;
struct fast_unop_func;

struct fast_const *fast_const_construct(enum table_column_type col_type,
                                        const void *value, struct dbms *dbms);

struct fast_column *fast_column_construct(const char *table_name,
                                          const char *column_name, struct dbms *dbms);

struct fast_unop *fast_unop_construct(void *parent, struct fast_unop_func *fuf,
                                      struct dbms *dbms);

struct fast_binop *fast_binop_construct(void *p_left, void *p_right,
                                        struct fast_binop_func *fbf, struct dbms *dbms);
