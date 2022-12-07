#pragma once

// current implementation differs from data_dist because it has less pushs and pops
// operations with table pages and tuples

#include "io/page/p_container.h"
#include "io/page/po_ptr.h"

struct dbms;
struct tp_tuple;
struct dp_tuple;

fileoff_t dbms_td_create_close(struct dbms *dbms);
void dbms_td_drop(struct dbms *dbms, const fileoff_t last);

bool dbms_td_pop_single(struct dbms *dbms, const struct dp_tuple *tuple,
                        fileoff_t *gappy_last, page_entry *pe_out);
void dbms_td_push_single(struct dbms *dbms, fileoff_t *gappy_last,
                         const page_entry *entry);
