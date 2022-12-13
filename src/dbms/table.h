#pragma once

#include <stddef.h>
#include <stdint.h>

struct tp_tuple;
struct dp_tuple;
struct tpt_col_info;
struct dbms;
struct dpt_column;
struct page_sso;

void tpt_memcpy_specific(void *dest, const void *src, const uint8_t col_type,
                         struct dbms *dbms);

void tpt_erase(const struct tp_tuple *tpt_old, const struct tp_tuple *tpt_new,
               const struct dp_tuple *dpt, const struct tpt_col_info *col_info,
               struct dbms *dbms);

void tpt_erase_columns(struct tp_tuple *tpt_old, struct tp_tuple *tpt_new,
                       const struct dp_tuple *dpt, const struct tpt_col_info *col_info,
                       size_t arr_size, size_t *idxs, struct dbms *dbms);
